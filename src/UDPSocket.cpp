//
// Created by CmST0us on 2019/1/14.
//

#include "UDPSocket.hpp"

using namespace socketkit;

UDPSocket::UDPSocket(asio::io_context &context, const socketkit::UDPEndpoint &endpoint,
                     unsigned short localPort)
     : _context(context),
     _socket(_context),
     _endpoint(endpoint),
     _localPort(localPort) {
}

UDPSocket::~UDPSocket() {
    _socket.close();
}

asio::io_context& UDPSocket::ioContext() {
    return _context;
}

void UDPSocket::connect() {
    auto listenEndpoint = asio::ip::udp::endpoint(asio::ip::udp::v4(), _localPort);
    _socket.open(listenEndpoint.protocol());
    _socket.bind(listenEndpoint);
}

void UDPSocket::read() {
    auto self(shared_from_this());
    _socket.async_receive_from(asio::buffer(_buffer), _senderEndpoint, [self](const std::error_code& ec, std::size_t size){
        asio::const_buffer buf(self->_buffer.data(), size);
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

void UDPSocket::write(const asio::const_buffer &data) {
    bool isWriting = !_writeQueue.empty();
    _writeQueue.emplace_back(data);
    if (!isWriting) {
        asyncWrite();
    }
}

void UDPSocket::error(const std::error_code &ec) {
    if (mErrorHandler != nullptr) {
        mErrorHandler(ec);
    }
}

void UDPSocket::close() {
    _socket.close();
}

void UDPSocket::closeWrite() {
    std::error_code ec;
    _socket.shutdown(asio::ip::udp::socket::shutdown_send, ec);
}

DataType UDPSocket::communicatorDataType() const {
    return DataType::Packet;
}

void UDPSocket::asyncWrite() {
    auto data = _writeQueue.front();
    auto self(shared_from_this());
    _socket.async_send_to(data, _endpoint.getEndpoint(), [self](const std::error_code& ec, std::size_t size) {
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

const UDPEndpoint UDPSocket::remoteEndpoint() {
    UDPEndpoint p(_context, _senderEndpoint);
    return p;
}