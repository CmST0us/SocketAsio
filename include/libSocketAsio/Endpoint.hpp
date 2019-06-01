//
// Created by CmST0us on 2019/1/12.
//

#pragma once

#include <string>
#include <iostream>

#include <asio.hpp>

namespace socketkit {
template <typename InternetProtocol>
class Endpoint {
private:
    asio::io_context& _context;
    asio::ip::basic_resolver_results<InternetProtocol> _endpoints;
    asio::ip::basic_endpoint<InternetProtocol> _endpoint;


    void resolve(const std::string& domain, const std::string& service) {
        asio::ip::basic_resolver<InternetProtocol> resolver(_context);
        _endpoints = resolver.resolve(domain, service);
        _endpoint = getFirstEndpoint();
    }

public:

    Endpoint(asio::io_context& context, const std::string& domain, const std::string& service, bool shouldResolve = true)
        : _context(context) {
        if (shouldResolve) {
            resolve(domain, service);
        }
    };

    explicit Endpoint(asio::io_context& context, asio::ip::basic_endpoint<InternetProtocol> endpoint)
        : _endpoint(endpoint),
        _context(context) {

    }

    ~Endpoint() = default;

    std::string getFirstEndpointIp() const {
        return _endpoint.address().to_string();
    };

    short getFirstEndpointPort() const {
        return _endpoint.port();
    }

    decltype(_endpoint) getEndpoint() const {
        return _endpoint;
    }

    decltype(_endpoints) getEndpoints() const {
        return _endpoints;
    }

private:
    asio::ip::basic_endpoint<InternetProtocol> getFirstEndpoint() const {
        return _endpoints->endpoint();
    }
};
    typedef Endpoint<asio::ip::tcp> TCPEndpoint;
    typedef Endpoint<asio::ip::udp> UDPEndpoint;
    typedef Endpoint<asio::ip::udp> NoResolveEndpoint;

}


