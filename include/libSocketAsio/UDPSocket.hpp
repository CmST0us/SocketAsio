//
// Created by CmST0us on 2019/1/14.
//

#pragma once
#include <list>
#include "CommunicatorInterface.hpp"
#include "Endpoint.hpp"
namespace socketkit {

class UDPSocket final : public std::enable_shared_from_this<UDPSocket>,
                            public IRemoteCommunicator {

public:
    ErrorHandler mErrorHandler{nullptr};
    ErrorHandler mConnectHandler{nullptr};
    ReadHandler mReadHandler{nullptr};
    WriteHandler mWriteHandler{nullptr};

    explicit UDPSocket(asio::io_context& context, const UDPEndpoint& endpoint, unsigned short localPort = 0);
    virtual ~UDPSocket();

    virtual asio::io_context& ioContext() override;
    virtual void read() override;
    virtual void write(const asio::const_buffer& data) override;
    virtual void error(const std::error_code& ec) override;
    virtual void closeWrite() override;
    virtual void close() override;

    virtual DataType communicatorDataType() const override;

    virtual void connect() override;

    const UDPEndpoint remoteEndpoint();
private:
    asio::io_context& _context;
    asio::ip::udp::socket _socket;

    UDPEndpoint _endpoint;
    asio::ip::udp::endpoint _senderEndpoint;

    std::array<char, 1500> _buffer;
    std::list<asio::const_buffer> _writeQueue;

    unsigned short _localPort{0};

    void asyncWrite();
};

};
