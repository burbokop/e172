#pragma once

#include "server.h"
#include <list>
#include <queue>
#include <src/abstracteventprovider.h>
#include <src/entityaddedobserver.h>

namespace e172 {

class ReadPackage;
class GameApplication;
class Networker;

class GameServer : public AbstractEventProvider, public EntityAddedObserver
{
    friend class Networker;
    struct Private
    {};

public:
    GameServer(GameApplication &app,
               Networker *networker,
               const std::shared_ptr<Server> &server,
               Private);

    void sync();

    // AbstractEventHandler interface
public:
    std::optional<Event> pullEvent() override;

    // EntityAddedObserver interface
protected:
    void entityAdded(const e172::ptr<Entity> &) override;

private:
    void refreshSockets();
    bool processEventPackage(ReadPackage &&package);

private:
    GameApplication &m_app;
    Networker *m_networker = nullptr;
    std::shared_ptr<Server> m_server;
    std::list<std::shared_ptr<Socket>> m_sockets;
    std::queue<Event> m_eventQueue;
    std::queue<ptr<Entity>> m_entityAddEventQueue;
};

} // namespace e172
