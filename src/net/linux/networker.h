#pragma once

#include "../networker.h"

namespace e172 {

class LinuxNetworkerImpl : public Networker
{
public:
    LinuxNetworkerImpl() = default;

    // Networker interface
public:
    virtual e172::Either<Error, std::shared_ptr<Server>> listen(uint16_t port) override;
    virtual e172::Either<Error, std::shared_ptr<Socket>> connect(
        uint16_t port, const std::string &address) override;
};

} // namespace e172
