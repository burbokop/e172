#include "socket.h"

#include <cstring>
#include <fcntl.h>
#include <unistd.h>

class LinuxSocketBufferizeException : public std::exception
{
public:
    LinuxSocketBufferizeException(int errnum)
        : m_errnum(errnum){};

    // exception interface
public:
    const char *what() const noexcept override { return strerror(m_errnum); }

private:
    int m_errnum;
};

class LinuxSocketWriteException : public std::exception
{
public:
    LinuxSocketWriteException(int errnum)
        : m_errnum(errnum){};

    // exception interface
public:
    const char *what() const noexcept override { return strerror(m_errnum); }

private:
    int m_errnum;
};

e172::LinuxSocketImpl::LinuxSocketImpl(int fd)
    : m_fd(fd)
{
    assert(fd >= 0);
    LinuxSocketImpl::setFdNonBlockingFlag(m_fd, true);
}

e172::LinuxSocketImpl::~LinuxSocketImpl()
{
    ::close(m_fd);
}

void e172::LinuxSocketImpl::setFdNonBlockingFlag(int fd, bool nbm)
{
    auto flags = fcntl(fd, F_GETFL, 0);
    if (nbm) {
        flags |= O_NONBLOCK;
    } else {
        flags &= !O_NONBLOCK;
    }
    fcntl(fd, F_SETFL, flags);
}

std::size_t e172::LinuxSocketImpl::bufferize()
{
    std::size_t bufferized = 0;
    while (true) {
        const auto c = bufferizeChunk();
        if (c == 0) {
            break;
        } else {
            bufferized += c;
        }
    }
    return bufferized;
}

std::size_t e172::LinuxSocketImpl::bytesAvailable() const
{
    return m_buf.len();
}

std::size_t e172::LinuxSocketImpl::read(uint8_t *dst, std::size_t size)
{
    const auto c = std::min(m_buf.len(), size);
    for (std::size_t i = 0; i < c; ++i) {
        dst[i] = m_buf.pop().value();
    }
    return c;
}

std::size_t e172::LinuxSocketImpl::peek(Byte *dst, std::size_t size) const
{
    return m_buf.peek(dst, size);
}

std::size_t e172::LinuxSocketImpl::write(const uint8_t *src, std::size_t size)
{
    const auto s = ::write(m_fd, src, size);
    if (s < 0) {
        switch (errno) {
        case ECONNRESET:
            m_isConnected = false;
            return 0;
        default:
            throw LinuxSocketWriteException(errno);
        }
    }
    return s;
}

void e172::LinuxSocketImpl::flush() {}

std::size_t e172::LinuxSocketImpl::bufferizeChunk()
{
    const auto sizeToRead = std::min(m_buf.push_ability(), s_chunkSize);
    if (sizeToRead == 0) {
        return 0;
    }

    std::uint8_t chunk[s_chunkSize];
    const auto size = ::read(m_fd, chunk, sizeToRead);
    if (size < 0) {
        switch (errno) {
        case EAGAIN:
            return 0;
        case ECONNRESET:
            m_isConnected = false;
            return 0;
        default:
            throw LinuxSocketBufferizeException(errno);
        }
    }
    for (std::size_t i = 0; i < size; ++i) {
        m_buf.push(chunk[i]);
    }
    return size;
}
