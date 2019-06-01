//
// Created by CmST0us on 2019/1/14.
//
#include <iostream>
#include "TCPAcceptor.hpp"

using namespace socketkit;
using namespace asio::ip;

TCPAcceptor::TCPAcceptor(asio::io_context &context, short port)
    : _context(context),
    _acceptor(_context, tcp::endpoint(tcp::v4(), port)) {

}

TCPAcceptor::~TCPAcceptor() {
    
}

asio::io_context& TCPAcceptor::ioContext() {
    return _context;
}

void TCPAcceptor::accept(const AcceptHandler& handler) {
    auto self(shared_from_this());
    _acceptor.async_accept(_context, [self, handler](const std::error_code& ec, tcp::socket socket) {
        if (!ec) {
            auto client = std::make_shared<TCPSocket>(socket.get_io_context(), std::move(socket));
            handler(ec, client);
            self->accept(handler);
        } else {
           self->_acceptor.close();
        }
    });
}
