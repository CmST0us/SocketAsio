
#include "TCPSocket.hpp"

using namespace socketkit;

TCPSocket::TCPSocket(asio::io_context& context, const TCPEndpoint& endpoint)
    : _stateMachine(CommunicatorType::Remote),
    _context(context),
    _endpoint(endpoint),
    _socket(_context),
    _connector(new TCPConnector(_context, _socket)) {


}

TCPSocket::TCPSocket(asio::io_context& context, asio::ip::tcp::socket socket)
    : _stateMachine(CommunicatorType::Local),
    _context(context),
    _socket(std::move(socket)),
    _endpoint(_context, _socket.remote_endpoint()) {

}

TCPSocket::~TCPSocket() {
    std::error_code ec;
    _socket.shutdown(asio::ip::tcp::socket::shutdown_send,ec);
    _socket.close(ec);
}

asio::io_context& TCPSocket::ioContext() {
    return _context;
}

void TCPSocket::read() {
    auto self(shared_from_this());
    asio::async_read(_socket, asio::buffer(_buffer), asio::transfer_at_least(1), [self](const std::error_code& ec, std::size_t bytes_transferred) {
         asio::const_buffer buf(self->_buffer.data(), bytes_transferred);
         if (!ec) {
             if (self->mReadHandler != nullptr) {
                 self->mReadHandler(ec, buf);
                 self->read();
             }
         } else {
             self->error(ec);
         }
    });
}

void TCPSocket::write(const asio::const_buffer& data) {
    bool isWriting = !_writeQueue.empty();
    _writeQueue.emplace_back(data);
    if (!isWriting) {
        asyncWrite();
    }
}

void TCPSocket::error(const std::error_code &ec) {
    if (mErrorHandler != nullptr) {
        mErrorHandler(ec);
    }
}

void TCPSocket::asyncWrite() {
    auto data = _writeQueue.front();
    auto self(shared_from_this());
    asio::async_write(_socket, data, [self](const std::error_code& ec, std::size_t size) {
        if (!ec) {
            self->_writeQueue.pop_front();
            if (self->mWriteHandler != nullptr) {
                self->mWriteHandler(ec, size);
            }
            if (!self->_writeQueue.empty()) {
                self->asyncWrite();
            }
        } else {
            self->error(ec);
            if (!self->_writeQueue.empty()) {
                self->_writeQueue.clear();
            }
        }
    });
}

void TCPSocket::closeWrite() {
    std::error_code ec;
   _socket.shutdown(asio::ip::tcp::socket::shutdown_send, ec);
}

void TCPSocket::close() {
    _socket.close();
}

const CommunicatorStateMachine& TCPSocket::stateMachine() const {
    return _stateMachine;
}

DataType TCPSocket::communicatorDataType() const {
    return DataType::Stream;
}

void TCPSocket::connect() {
    _connector->connect(_endpoint, mConnectHandler);
}

void TCPSocket::open() {

}

void TCPSocket::continueFinished() {
    this->read();
}

