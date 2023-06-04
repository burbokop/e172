#pragma once

#include "../../socket.h"

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
    virtual std::size_t bufferize() override;
    virtual std::size_t bytesReadyRead() override;
    virtual std::size_t read(uint8_t *dst, std::size_t size) override;
    virtual std::size_t write(const uint8_t *src, std::size_t size) override;
    virtual bool isConnected() const override;

private:
    int m_fd;
};

} // namespace e172
