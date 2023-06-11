#include "networker.h"

#include "server.h"
#include "socket.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

namespace e172 {

Either<Networker::Error, std::shared_ptr<Server>> LinuxNetworkerImpl::listen(uint16_t port)
{
    const auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return Left(FailedToCreateSocket);
    }

    sockaddr_in servaddr;
    ::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if ((::bind(fd, reinterpret_cast<sockaddr *>(&servaddr), sizeof(servaddr))) != 0) {
        switch (errno) {
        case EADDRINUSE:
            return Left(AddressAlreadyInUse);
        default:
            return Left(UnwnownBindingError);
        }
    }

    if ((::listen(fd, 5)) != 0) {
        return Left(ListenFailed);
    }
    return Right<std::shared_ptr<Server>>(std::make_shared<LinuxServerImpl>(fd));
}

Either<Networker::Error, std::shared_ptr<Socket>> e172::LinuxNetworkerImpl::connect(
    uint16_t port, const std::string &address)
{
    const auto fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        return Left(FailedToCreateSocket);
    }

    sockaddr_in servaddr;
    ::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = ::inet_addr(address.c_str());
    servaddr.sin_port = ::htons(port);

    if (::connect(fd, reinterpret_cast<sockaddr *>(&servaddr), sizeof(servaddr)) != 0) {
        switch (errno) {
        case ECONNREFUSED:
            return Left(ConnectionRefused);
        default:
            return Left(UnwnownConnectionError);
        }
    }
    return Right<std::shared_ptr<Socket>>(std::make_shared<LinuxSocketImpl>(fd));
}

} // namespace e172
