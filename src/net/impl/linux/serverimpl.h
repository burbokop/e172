#pragma once

#include "../../server.h"

namespace e172 {

class LinuxServerImpl : public Server
{
public:
    LinuxServerImpl(int fd);

    ~LinuxServerImpl();

    // Server interface
public:
    virtual std::shared_ptr<Socket> pullConnection() override;

private:
    int m_fd;
};

} // namespace e172
