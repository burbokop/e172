#include "socketimpl.h"

#include <cstring>
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

e172::LinuxSocketImpl::~LinuxSocketImpl()
{
    // close the socket
    ::close(m_fd);
}

std::size_t e172::LinuxSocketImpl::bufferize()
{
    std::uint8_t chunk[s_chunkSize];
    const auto size = ::read(m_fd, chunk, s_chunkSize);
    if (size < 0) {
        throw LinuxSocketBufferizeException(errno);
    }
    for (std::size_t i = 0; i < size; ++i) {
        m_buf.push(chunk[i]);
    }
    return size;
}

std::size_t e172::LinuxSocketImpl::bytesAvailable() const
{
    return m_buf.size();
}

std::size_t e172::LinuxSocketImpl::read(uint8_t *dst, std::size_t size)
{
    todo;
}

std::size_t e172::LinuxSocketImpl::peek(Byte *dst, std::size_t size) const
{
    todo;
}

std::size_t e172::LinuxSocketImpl::write(const uint8_t *src, std::size_t size)
{
    todo;
}

