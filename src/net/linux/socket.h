#pragma once

#include "../socket.h"
#include "src/utility/ringbuf.h"

namespace e172 {

/**
 * @brief The LinuxSocket class implements linux/unix tcp socket
 */
class LinuxSocket : public Socket
{
public:
    LinuxSocket(int fd);

    ~LinuxSocket();

    static void setFdNonBlockingFlag(int fd, bool nbm);

    // Socket interface
public:
    std::size_t bufferize() override;
    std::size_t bytesAvailable() const override;
    std::size_t read(uint8_t *dst, std::size_t size) override;
    std::size_t peek(Byte *dst, std::size_t size) const override;
    std::size_t write(const uint8_t *src, std::size_t size) override;
    void flush() override;
    bool isConnected() const override { return m_isConnected; }

private:
    std::size_t bufferizeChunk();

private:
    static constexpr std::size_t s_chunkSize = 256;

    int m_fd;
    bool m_isConnected = true;
    RingBuf<Byte, 4096> m_buf;
};

} // namespace e172
