#include <asio.hpp>
#include <iostream>
#include "Endpoint.hpp"
#include "TCPConnector.hpp"
int main(int argc, char *argv[]) {
    asio::io_context context;
    socketkit::TCPConnector connector(context);
    connector.connect("127.0.0.1", "12002", [](std::error_code ec, asio::ip::tcp::socket& socket) {
        if (!ec) {
            std::cout<<"connect success"<<std::endl;
        }
    });
    context.run();
    return 0;

}