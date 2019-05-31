#include "Endpoint.hpp"
#include "TCPConnector.hpp"

using namespace socketkit;

TCPConnector::TCPConnector(asio::io_context& context)
    : _context(context),
    _socket(_context) {

}

TCPConnector::~TCPConnector() {
}

asio::io_context& TCPConnector::ioContext() {
    return _context;
}

void TCPConnector::connect(const TCPEndpoint &endpoint,
        const TCPConnectorHandler& handler) {
   _socket.async_connect(endpoint.getFirstEndpoint(), [this, handler](std::error_code ec) {
       if (handler != nullptr) {
           handler(ec, _socket);
       }
   });
}

void TCPConnector::connect(const std::string& domain,
        const std::string& port,
        const TCPConnectorHandler& handler) {
    TCPEndpoint ep(_context, domain, port);
    this->connect(ep, handler);
}

