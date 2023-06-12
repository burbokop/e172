#pragma once

#include "common.h"
#include "socket.h"
#include <memory>
#include <src/time/elapsedtimer.h>

namespace e172 {

class GameApplication;
class ReadPackage;
class Networker;

/**
 * @brief The GameClient class used to make client/server application.
 * It sends events to server and receives entity statuses.
 * Example can be found in `e172::integration::ClientServerSpec`
 */
class GameClient
{
    friend class Networker;
    struct Private
    {};

public:
    /**
     * @brief GameClient - private ctor. Use e172::Networker to create it
     */
    GameClient(GameApplication &app,
               Networker *networker,
               const std::shared_ptr<Socket> &socket,
               Private)
        : m_app(app)
        , m_networker(networker)
        , m_socket(socket)
    {}

    /**
     * @brief isConnected
     * @return true if client still connected to server
     */
    bool isConnected() const { return m_socket != nullptr; }

    /**
     * @brief sync - synchronize with server
     * Send all pending packages, receive and process all incoming packages.
     * Should be called releatedly with equal intervals until disconnect
     */
    void sync();

    Statistics statistics() const { return m_statistics; }

private:
    bool processInitPackage(ReadPackage &&package);
    bool processAddEntityPackage(ReadPackage &&package);
    bool processRemoveEntityPackage(ReadPackage &&package);
    bool processSyncEntityPackage(ReadPackage &&package);

private:
    GameApplication &m_app;
    Networker *m_networker = nullptr;
    std::shared_ptr<Socket> m_socket;
    std::optional<PackedClientId> m_clientId;

    Statistics m_statistics;
    Statistics m_incompleatedStatistics;
    ElapsedTimer m_statisticsTimer = 1000;
};

} // namespace e172
