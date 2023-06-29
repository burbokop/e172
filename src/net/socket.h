// Copyright 2023 Borys Boiko

#pragma once

#include "../utility/io.h"
#include <cstdint>
#include <optional>
#include <vector>

namespace e172 {

/**
 * @brief The Socket class - abstract network socket
 * To create use e172::Networker or e172::Server specific implementation 
 */
class Socket : public Read, public Write
{
public:
    Socket() = default;

    virtual bool isConnected() const = 0;
};

} // namespace e172
