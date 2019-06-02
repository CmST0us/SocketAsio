//
// Created by CmST0us on 2019/1/14.
//

#pragma once

#include <memory>
#include <functional>
#include <asio/ip/tcp.hpp>

#include "AsyncInterface.hpp"
#include "TCPSocket.hpp"

namespace socketkit {


class TCPAcceptor final : public std::enable_shared_from_this<TCPAcceptor>, public IAsync {
public:
    TCPAcceptor(asio::io_context& context, unsigned short port);
    virtual ~TCPAcceptor();
    virtual asio::io_context& ioContext() override;

    TCPAcceptor(const TCPAcceptor&) = delete;
    TCPAcceptor& operator=(const TCPAcceptor&) = delete;

    using AcceptHandler = std::function<void(const std::error_code& ec, std::shared_ptr<TCPSocket> socket)>;

    void accept(const AcceptHandler& handler);

private:
    asio::io_context& _context;
    asio::ip::tcp::acceptor _acceptor;
};

};
