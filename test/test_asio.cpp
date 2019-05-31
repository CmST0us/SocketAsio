#include <asio.hpp>
#include <iostream>
#include "TCPAcceptor.hpp"


int main(int argc, char *argv[]) {
    asio::io_context context;
    socketkit::TCPAcceptor accptor(context, 12001);
    accptor.accept([](std::error_code ec, int s){

    });
    context.run();
    return 0;

}