//
// Created by CmST0us on 2019/1/14.
//

#pragma once

#include <asio.hpp>
#include "AsyncInterface.hpp"

namespace socketkit {

class TCPConnector final : public IAsync {

public:
    explicit TCPConnector(asio::io_context& context) ;
    virtual ~TCPConnector() override;
    virtual asio::io_context& ioContext() override;

    TCPConnector(const TCPConnector&) = delete;
    TCPConnector& operator=(const TCPConnector&) = delete;

public:
//    int mTimeoutSecond{5};  //单次连接超时时间
//    int mRetrySecond{2};    //失败重连时间
//    int mRetryTimes{3};     //重试次数

    using TCPConnectorHandler = std::function<void(std::error_code ec, asio::ip::tcp::socket& socket)>;

    void connect(const TCPEndpoint& endpoint, const TCPConnectorHandler& handler);
    void connect(const std::string& domain, const std::string& port, const TCPConnectorHandler& handler);

private:
    asio::io_context& _context;
    asio::ip::tcp::socket _socket;
};

};
