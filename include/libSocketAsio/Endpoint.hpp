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
    asio::ip::basic_resolver<InternetProtocol> _resolver;
    asio::ip::basic_resolver_results<InternetProtocol> _endpoints;
    std::string _endpointDomain{"localhost"};
    std::string _endpointServiceName{"0"};


    void resolve() {
        _endpoints = _resolver.resolve(_endpointDomain, _endpointServiceName);
    }

public:

    Endpoint(asio::io_context& context, const std::string& domain, const std::string& service, bool shouldResolve = true)
        :   _endpointDomain{domain},
        _endpointServiceName{service},
        _context(context),
        _resolver(_context) {
        if (shouldResolve) {
            resolve();
        }
    };

    ~Endpoint() = default;

    std::string getFirstEndpointIp() const {
        return getFirstEndpoint().address().to_string();
    };

    std::string getEndpointDomain() const {
        return _endpointDomain;
    };

    std::string getFirstEndpointServiceName() const {
        return _endpoints->service_name();
    };

    short getFirstEndpointPort() const {
        return getFirstEndpoint().port();
    }

    asio::ip::basic_endpoint<InternetProtocol> getFirstEndpoint() const {
        return _endpoints->endpoint();
    }

    decltype(_endpoints) getEndpoints() const {
        return _endpoints;
    }
};
    typedef Endpoint<asio::ip::tcp> TCPEndpoint;
    typedef Endpoint<asio::ip::udp> UDPEndpoint;
    typedef Endpoint<asio::ip::udp> NoResolveEndpoint;

}


