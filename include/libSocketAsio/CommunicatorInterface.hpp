//
//  CommunicatorInterface.hpp
//  SocketKit
//
//  Created by CmST0us on 2018/10/12.
//  Copyright © 2018年 CmST0us. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <memory>
#include <asio.hpp>

#include "AsyncInterface.hpp"
#include "Endpoint.hpp"

namespace socketkit
{

enum class DataType {
    Stream,
    Packet
};

class ICommunicator: public IAsync {
public:
    using ReadHandler = std::function<void(const std::error_code&, const asio::const_buffer& data)>;
    using WriteHandler = std::function<void(const std::error_code&, std::size_t length)>;
    using ErrorHandler = std::function<void(const std::error_code&)>;

    virtual ~ICommunicator() = default;
    virtual void read() = 0;
    virtual void write(const asio::const_buffer& data) = 0;
    virtual void error(const std::error_code& ec) = 0;
    virtual void closeWrite() = 0;
    virtual void close() = 0;

    virtual DataType communicatorDataType() const = 0;
};

class IRemoteCommunicator : public ICommunicator {
public:
    virtual ~IRemoteCommunicator() = default;
    virtual void connect() = 0;
};

class ILocalCommunicator : public ICommunicator {
public:
    virtual ~ILocalCommunicator() = default;

    virtual void open() = 0;
    virtual void continueFinished() = 0;
};

};

