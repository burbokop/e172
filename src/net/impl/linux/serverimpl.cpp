#include "serverimpl.h"
#include "socketimpl.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

std::shared_ptr<e172::Socket> e172::LinuxServerImpl::pullConnection()
{
    sockaddr_in cli;
    socklen_t len = sizeof(cli);

    // TODO make nonblocking
    const auto connfd = ::accept(m_fd, reinterpret_cast<sockaddr *>(&cli), &len);
    if (connfd < 0) {
        return nullptr;
    } else {
        return std::make_shared<LinuxSocketImpl>(connfd);
    }
}

e172::LinuxServerImpl::~LinuxServerImpl()
{
    close(m_fd);
}
