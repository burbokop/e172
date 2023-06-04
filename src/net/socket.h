#pragma once

#include <cstdint>
#include <vector>

#include <src/utility/buffer.h>

namespace e172 {

class Socket
{
public:
    Socket() = default;

    std::size_t write(const std::vector<std::uint8_t> &bytes)
    {
        return write(bytes.data(), bytes.size());
    }

    std::size_t write(std::function<void(WritePackage)> writeFn)
    {
        const auto b = pack(writeFn);
        return write(b.data(), b.size());
    }

    virtual std::size_t bufferize() = 0;
    virtual std::size_t bytesReadyRead() = 0;
    virtual std::size_t read(std::uint8_t *dst, std::size_t size) = 0;
    virtual std::size_t write(const std::uint8_t *src, std::size_t size) = 0;
    virtual bool isConnected() const = 0;

    virtual ~Socket() = default;
};

} // namespace e172
