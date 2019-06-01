#include "Endpoint.hpp"
#include "TCPConnector.hpp"

using namespace socketkit;

TCPConnector::TCPConnector(asio::io_context& context, asio::ip::tcp::socket& socket)
    : _context(context),
    _socket(socket) {

}

TCPConnector::~TCPConnector() {
}

asio::io_context& TCPConnector::ioContext() {
    return _context;
}

void TCPConnector::connect(const TCPEndpoint &endpoint,
        const ICommunicator::ErrorHandler& handler) {
   asio::async_connect(_socket, endpoint.getEndpoints(), [handler](const std::error_code& ec, const asio::ip::tcp::endpoint& ep) {
       handler(ec);
   });
}

void TCPConnector::connect(const std::string& domain,
        const std::string& port,
        const ICommunicator::ErrorHandler& handler) {
    TCPEndpoint ep(_context, domain, port);
    this->connect(ep, handler);
}

