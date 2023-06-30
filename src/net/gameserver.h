// Copyright 2023 Borys Boiko

#pragma once

#include "../abstracteventprovider.h"
#include "../entitylifetimeobserver.h"
#include "../time/elapsedtimer.h"
#include "../utility/signal.h"
#include "common.h"
#include "server.h"
#include <list>
#include <memory>
#include <queue>

namespace e172 {

class ReadPackage;
class GameApplication;
class Networker;

/**
 * @brief The GameServer class used to make client/server application.
 * Manages connections list. broadcusting entity states. receives events from all clients and provides them through e172::AbstractEventProvider interface
 * Example can be found in `e172::integration::ClientServerSpec`
 */
class GameServer : public AbstractEventProvider, public EntityLifeTimeObserver
{
    friend class Networker;
    struct Private
    {};

public:
    /**
     * @brief GameServer - private ctor. Use e172::Networker to create it
     */
    GameServer(GameApplication &app,
               Networker *networker,
               const std::shared_ptr<Server> &server,
               Private);

    /**
     * @brief sync - synchronize with server
     * Send all pending packages, receive and process all incoming packages.
     * Should be called releatedly with equal intervals until disconnect
     */
    void sync();

    /**
     * @brief clientConnected - signal emited when client connected
     * @return signal object to make connection to it
     */
    auto &clientConnected() { return m_clientConnected; }

    /**
     * @brief clientDisconnected - signal emited when client disconnected
     * @return signal object to make connection to it
     */
    auto &clientDisconnected() { return m_clientDisconnected; }

    /**
     * @brief sendCustomPackage - send custom package to client with provided id
     * Signature of this function is identical to `WritePackage::push` except `clientId`
     * @param clientId - clinet to send package
     * @param type
     * @param writeFn
     * @return num of bytes writen or 0 if client not found or not valid
     */
    std::size_t sendCustomPackage(PackedClientId clientId,
                                  PackageType type,
                                  const std::function<void(WritePackage)> &writeFn);

    void broadcastCustomPackage(PackageType type, const std::function<void(WritePackage)> &writeFn);

    Statistics statistics() const { return m_statistics; }
    GameApplication &app() { return m_app; };

    // AbstractEventHandler interface
public:
    std::optional<Event> pullEvent() override;

    // EntityAddedObserver interface
protected:
    void entityAdded(const e172::ptr<Entity> &) override;
    void entityRemoved(const Entity::Id &) override;

private:
    /**
     * @brief refreshSockets
     * @return num connected after refresh
     */
    std::size_t refreshSockets();
    bool processEventPackage(ReadPackage &&package);

    void broadcastEntityAddedPackage(const ptr<Entity> &entity);
    void sendEntityAddedPackage(Socket &s, const ptr<Entity> &entity);

private:
    struct Client
    {
        PackedClientId id;
        std::shared_ptr<Socket> socket;
    };

private:
    GameApplication &m_app;
    Networker *m_networker = nullptr;
    std::shared_ptr<Server> m_server;
    std::list<Client> m_clients;
    std::queue<Event> m_eventQueue;
    std::queue<ptr<Entity>> m_entityAddEventQueue;
    std::queue<Entity::Id> m_entityRemoveEventQueue;
    PackedClientId m_nextClientId = 0;
    Signal<void(PackedClientId), Private> m_clientConnected;
    Signal<void(PackedClientId), Private> m_clientDisconnected;

    Statistics m_statistics;
    Statistics m_incompleatedStatistics;
    ElapsedTimer m_statisticsTimer = 1000;
};

} // namespace e172
