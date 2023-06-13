#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <src/utility/buffer.h>
#include <src/utility/io.h>

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
