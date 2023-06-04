#pragma once

#include "socket.h"
#include <memory>

namespace e172 {

class Server
{
public:
    Server() = default;

    virtual std::shared_ptr<Socket> pullConnection() = 0;

    virtual ~Server() = default;
};

} // namespace e172
