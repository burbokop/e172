#include "server.h"

#include "socket.h"
#include <netdb.h>

std::shared_ptr<e172::Socket> e172::LinuxServer::pullConnection()
{
    sockaddr_in cli;
    socklen_t len = sizeof(cli);

    const auto connfd = ::accept(m_fd, reinterpret_cast<sockaddr *>(&cli), &len);
    if (connfd < 0) {
        return nullptr;
    } else {
        return std::make_shared<LinuxSocket>(connfd);
    }
}

e172::LinuxServer::LinuxServer(int fd)
    : m_fd(fd)
{
    assert(fd >= 0);
    LinuxSocket::setFdNonBlockingFlag(m_fd, true);
}

e172::LinuxServer::~LinuxServer()
{
    close(m_fd);
}
