#pragma once

#include "server.h"
#include <list>
#include <queue>
#include <src/abstracteventprovider.h>

namespace e172 {

class ReadPackage;
class GameApplication;

class GameServer : public AbstractEventProvider
{
    friend class Networker;
    struct Private
    {};

public:
    GameServer(GameApplication &app, const std::shared_ptr<Server> &server, Private)
        : m_app(app)
        , m_server(server)
    {}

    void sync();

    // AbstractEventHandler interface
public:
    std::optional<Event> pullEvent() override;

private:
    void refreshSockets();
    void processEventPackage(ReadPackage &&package);

private:
    GameApplication &m_app;
    std::shared_ptr<Server> m_server;
    std::list<std::shared_ptr<Socket>> m_sockets;
    std::queue<Event> m_eventQueue;
};

} // namespace e172
