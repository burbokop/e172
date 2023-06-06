#pragma once

#include "socket.h"
#include <memory>

namespace e172 {

class GameApplication;
class ReadPackage;

class GameClient
{
    friend class Networker;
    struct Private
    {};

public:
    GameClient(GameApplication &app, const std::shared_ptr<Socket> &socket, Private)
        : m_app(app)
        , m_socket(socket)
    {}

    bool isConnected() const { return m_socket != nullptr; }

    void sync();

private:
    void syncEntity(ReadPackage &&package);

private:
    GameApplication &m_app;
    std::shared_ptr<Socket> m_socket;
};

} // namespace e172
