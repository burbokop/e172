// Copyright 2023 Borys Boiko

#pragma once

#include "socket.h"
#include <memory>

namespace e172 {

/**
 * @brief The Server class - abstract network server
 * To create use e172::Networker specific implementation
 */
class Server
{
public:
    Server() = default;

    virtual std::shared_ptr<Socket> pullConnection() = 0;

    virtual ~Server() = default;
};

} // namespace e172
