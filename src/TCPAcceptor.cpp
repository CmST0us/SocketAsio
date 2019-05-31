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
    _acceptor.async_accept(_context, [this, handler](std::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::cout<<"[TCPAcceptor]: Accept socket"<<std::endl;

        }
        this->accept(handler);
    });
}
