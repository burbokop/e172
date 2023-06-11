#pragma once

#include "../server.h"

namespace e172 {

class LinuxServer : public Server
{
public:
    LinuxServer(int fd);

    ~LinuxServer();

    // Server interface
public:
    virtual std::shared_ptr<Socket> pullConnection() override;

private:
    int m_fd;
};

} // namespace e172
