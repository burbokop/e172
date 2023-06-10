#pragma once

#include "socket.h"
#include <memory>

namespace e172 {

class GameApplication;
class ReadPackage;
class Networker;

class GameClient
{
    friend class Networker;
    struct Private
    {};

public:
    GameClient(GameApplication &app,
               Networker *networker,
               const std::shared_ptr<Socket> &socket,
               Private)
        : m_app(app)
        , m_networker(networker)
        , m_socket(socket)
    {}

    bool isConnected() const { return m_socket != nullptr; }

    void sync();

private:
    bool processAddEntityPackage(ReadPackage &&package);
    bool processRemoveEntityPackage(ReadPackage &&package);
    bool processSyncEntityPackage(ReadPackage &&package);

private:
    GameApplication &m_app;
    Networker *m_networker = nullptr;
    std::shared_ptr<Socket> m_socket;
};

} // namespace e172
