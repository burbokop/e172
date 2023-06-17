// Copyright 2023 Borys Boiko

#pragma once

#include "../server.h"
#include <memory>

namespace e172 {

/**
 * @brief The LinuxServer class implements linux/unix tcp server
 */
class LinuxServer : public Server
{
public:
    LinuxServer(int fd);

    ~LinuxServer();

    // Server interface
public:
    std::shared_ptr<Socket> pullConnection() override;

private:
    int m_fd;
};

} // namespace e172
