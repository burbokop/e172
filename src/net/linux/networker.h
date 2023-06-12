#pragma once

#include "../networker.h"

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
    virtual e172::Either<Error, std::shared_ptr<Server>> listen(uint16_t port) override;
    virtual e172::Either<Error, std::shared_ptr<Socket>> connect(
        uint16_t port, const std::string &address) override;
};

} // namespace e172
