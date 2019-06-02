//
// Created by CmST0us on 2019/1/14.
//

#pragma once

#include <memory>
#include <list>
#include <asio.hpp>
#include "TCPConnector.hpp"
#include "CommunicatorInterface.hpp"

namespace socketkit {

class TCPSocket final : public std::enable_shared_from_this<TCPSocket>,
                         public IRemoteCommunicator,
                         public ILocalCommunicator {

public:
    ErrorHandler mErrorHandler{nullptr};
    ErrorHandler mConnectHandler{nullptr};
    ReadHandler mReadHandler{nullptr};
    WriteHandler mWriteHandler{nullptr};

    virtual asio::io_context& ioContext() override;
    virtual void read() override;
    virtual void write(const asio::const_buffer& data) override;
    virtual void error(const std::error_code& ec) override;
    virtual void closeWrite() override;
    virtual void close() override;

    virtual DataType communicatorDataType() const override;

    // Remote
    TCPSocket(asio::io_context& context, const TCPEndpoint& endpoint);
    virtual void connect() override;

    // Local
    TCPSocket(asio::io_context& context, asio::ip::tcp::socket socket);
    virtual void open() override;
    virtual void continueFinished() override;

    virtual ~TCPSocket();

private:
    asio::io_context& _context;
    asio::ip::tcp::socket _socket;
    std::unique_ptr<TCPConnector> _connector;

    TCPEndpoint _endpoint;

    std::array<char, 8192> _buffer;
    std::list<asio::const_buffer> _writeQueue;

    void asyncWrite();
};

};
