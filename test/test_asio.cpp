#include <asio.hpp>
#include <iostream>
#include "Endpoint.hpp"

int main(int argc, char *argv[]) {
    asio::io_context context;
    socketkit::TCPEndpoint tcpep(context, "www.baidu.com", "http");
    std::cout<<tcpep.getFirstEndpointIp()<<std::to_string(tcpep.getFirstEndpointPort());
    context.run();
    return 0;

}