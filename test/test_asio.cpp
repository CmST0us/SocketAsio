#include <asio.hpp>
#include <iostream>
#include <memory>
#include "Endpoint.hpp"
#include "TCPSocket.hpp"
#include "TCPAcceptor.hpp"
#include "UDPSocket.hpp"
void test_connect() {
    asio::io_context context;
    std::shared_ptr<socketkit::TCPSocket> socket(new socketkit::TCPSocket(context, socketkit::TCPEndpoint(context, "127.0.0.1", "12002")));

    socket->mConnectHandler = [socket](const std::error_code& ec) {
        if (!ec) {
            std::cout<<"connect success"<<std::endl;
            socket->read();
        } else {
            std::cout<<"error:"<<ec.message();
        }
    };
    socket->mReadHandler = [socket](const std::error_code& ec, const asio::const_buffer& data) {
         socket->write(data);
    };

    socket->mErrorHandler = [socket](const std::error_code& ec) {
        std::cout<<"error: "<<ec.message()<<std::endl;
    };
    socket->connect();
    context.run();
}

void test_accept() {
    asio::io_context context;
    auto acceptor = std::make_shared<socketkit::TCPAcceptor>(context, 12003);
    acceptor->accept([](const std::error_code& ec, std::shared_ptr<socketkit::TCPSocket> socket) {
        socket->mReadHandler = [socket](const std::error_code& ec, const asio::const_buffer& data) {
            socket->write(data);
        };
        socket->mErrorHandler = [socket](const std::error_code& ec) {
             std::cout<<"error: "<<ec.message()<<std::endl;
        };
        socket->open();
        socket->continueFinished();
    });
    context.run();
}

void test_udp() {
    asio::io_context context;
    auto udp = std::make_shared<socketkit::UDPSocket>(context, socketkit::UDPEndpoint(context, "127.0.0.1", "12005"), 12007);
    udp->mReadHandler = [udp](const std::error_code& ec, const asio::const_buffer& data) {
        udp->write(data);
    };

    udp->mErrorHandler = [udp](const std::error_code& ec) {
        std::cout<<"error: "<<ec.message()<<std::endl;
    };
    udp->connect();
    udp->read();
    context.run();
}

int main(int argc, char *argv[]) {
    test_udp();
    return 0;

}