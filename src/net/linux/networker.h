// Copyright 2023 Borys Boiko

#pragma once

#include "../networker.h"
#include <memory>
#include <string>

namespace e172 {

/**
 * @brief The LinuxServer class implements linux/unix tcp socket/server layer and works as factory of servers and sockets
 */
class LinuxNetworker : public Networker
{
public:
    LinuxNetworker() = default;

    // Networker interface
public:
    e172::Either<Error, std::shared_ptr<Server>> listen(std::uint16_t port) override;
    e172::Either<Error, std::shared_ptr<Socket>> connect(std::uint16_t port,
                                                         const std::string &address) override;
};

} // namespace e172
