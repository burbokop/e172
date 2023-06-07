#pragma once

#include "../../socket.h"

#include <queue>

namespace e172 {

class LinuxSocketImpl : public Socket
{
public:
    LinuxSocketImpl(int fd)
        : m_fd(fd)
    {
        assert(fd >= 0);
    }

    ~LinuxSocketImpl();

    // Socket interface
public:
    std::size_t bufferize() override;
    std::size_t bytesAvailable() const override;
    std::size_t read(uint8_t *dst, std::size_t size) override;
    std::size_t peek(Byte *dst, std::size_t size) const override;
    std::size_t write(const uint8_t *src, std::size_t size) override;
    bool isConnected() const override { return m_isConnected; }

private:
    static constexpr std::size_t s_chunkSize = 256;

    int m_fd;
    bool m_isConnected = true;
    std::queue<std::uint8_t> m_buf;
};

} // namespace e172
