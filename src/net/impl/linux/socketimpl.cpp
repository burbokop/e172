#include "socketimpl.h"

#include <unistd.h>

e172::LinuxSocketImpl::~LinuxSocketImpl()
{
    // close the socket
    ::close(m_fd);
}

std::size_t e172::LinuxSocketImpl::bufferize()
{
    todo;
}

std::size_t e172::LinuxSocketImpl::bytesAvailable() const
{
    todo;
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

bool e172::LinuxSocketImpl::isConnected() const
{
    todo;
}
