#include "server.h"

#include "socket.h"
#include <netdb.h>

std::shared_ptr<e172::Socket> e172::LinuxServerImpl::pullConnection()
{
    sockaddr_in cli;
    socklen_t len = sizeof(cli);

    const auto connfd = ::accept(m_fd, reinterpret_cast<sockaddr *>(&cli), &len);
    if (connfd < 0) {
        return nullptr;
    } else {
        return std::make_shared<LinuxSocketImpl>(connfd);
    }
}

e172::LinuxServerImpl::LinuxServerImpl(int fd)
    : m_fd(fd)
{
    assert(fd >= 0);
    LinuxSocketImpl::setFdNonBlockingFlag(m_fd, true);
}

e172::LinuxServerImpl::~LinuxServerImpl()
{
    close(m_fd);
}
