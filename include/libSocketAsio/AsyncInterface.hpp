
#pragma once

#include <asio/io_context.hpp>

namespace socketkit {

class IAsync {
public:
    virtual ~IAsync() = default;
    virtual asio::io_context& ioContext() = 0;
};

};
