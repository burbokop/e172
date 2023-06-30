// Copyright 2023 Borys Boiko

#pragma once

#include "../networker.h"
#include "server.h"
#include "socket.h"
#include <map>
#include <memory>
#include <string>

namespace e172 {

/**
 * @brief The MemNetworker class implements internal application memory communication server/socket layer
 * Can be used for tests or to simulate some real communication
 */
class MemNetworker : public Networker
{
public:
    MemNetworker() = default;

    // Networker interface
public:
    Either<Error, std::shared_ptr<Server>> listen(std::uint16_t port) override
    {
        std::lock_guard lock(m_mutex);
        auto it = m_servers.find(port);
        if (it == m_servers.end()) {
            auto s = std::make_shared<MemServer>();
            it = m_servers.insert(it, {port, s});
            return Right(std::dynamic_pointer_cast<Server>(s));
        } else {
            return Left(AddressAlreadyInUse);
        }
    }

    Either<Error, std::shared_ptr<Socket>> connect(std::uint16_t port,
                                                   const std::string &address = Localhost) override
    {
        std::lock_guard lock(m_mutex);
        auto it = m_servers.find(port);
        if (it != m_servers.end()) {
            auto serverChannel = MemSocket::Channel::make();
            auto serverSocket = std::make_shared<MemSocket>(serverChannel);
            auto clientSocket = std::make_shared<MemSocket>(serverChannel.inverted());

            it->second->pushConnection(serverSocket);
            return Right(std::dynamic_pointer_cast<Socket>(clientSocket));
        } else {
            return Left(ConnectionRefused);
        }
    }

private:
    std::map<std::uint16_t, std::shared_ptr<MemServer>> m_servers;
    std::mutex m_mutex;
};

} // namespace e172
