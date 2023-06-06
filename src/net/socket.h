#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <src/utility/buffer.h>
#include <src/utility/io.h>

namespace e172 {

class Socket : public Read, public Write
{
public:
    Socket() = default;

    virtual bool isConnected() const = 0;
};

} // namespace e172
