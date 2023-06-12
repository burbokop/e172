#include "gameserver.h"
#include "../gameapplication.h"
#include "common.h"
#include "networker.h"
#include "src/utility/package.h"
#include <src/debug.h>

e172::GameServer::GameServer(GameApplication &app,
                             Networker *networker,
                             const std::shared_ptr<Server> &server,
                             Private)
    : m_app(app)
    , m_networker(networker)
    , m_server(server)
{
    assert(server);
}

void e172::GameServer::sync()
{
    assert(m_networker);
    refreshSockets();

    while (!m_entityAddEventQueue.empty()) {
        const auto &entity = m_entityAddEventQueue.front();
        broadcastEntityAddedPackage(entity);
        m_entityAddEventQueue.pop();
    }

    while (!m_entityRemoveEventQueue.empty()) {
        const auto id = m_entityRemoveEventQueue.front();
        for (const auto &client : m_clients) {
            if (client.socket->isConnected()) {
                m_incompleatedStatistics.bytesWritenPerSecond
                    += WritePackage::push(*client.socket,
                                          PackageType(GamePackageType::RemoveEntity),
                                          [id](WritePackage p) { p.write(PackedEntityId(id)); });
            }
        }
        m_entityRemoveEventQueue.pop();
    }

    for (const auto &e : m_app.entities()) {
        if (e->needSyncNet()) {
            const auto id = e->entityId();
            WriteBuffer buf;
            e->writeNet(buf);

            for (const auto &client : m_clients) {
                if (client.socket->isConnected()) {
                    m_incompleatedStatistics.bytesWritenPerSecond
                        += WritePackage::push(*client.socket,
                                              PackageType(GamePackageType::SyncEntity),
                                              [id, &buf](WritePackage p) {
                                                  p.write(PackedEntityId(id));
                                                  p.write(std::move(buf));
                                              });
                }
            }
        }
    }

    for (const auto &client : m_clients) {
        //m_incompleatedStatistics.bytesWritenPerSecond
        //    += WritePackage::push(*client.socket,
        //                          PackageType(GamePackageType::SyncEntity),
        //                          [](WritePackage p) {
        //                              Bytes b(3000, 0);
        //                              p.write(std::move(b));
        //                          });

        client.socket->flush();
    }

    for (const auto &client : m_clients) {
        while (true) {
            const auto size = ReadPackage::pull(*client.socket, [this](ReadPackage package) {
                switch (GamePackageType(package.type())) {
                case GamePackageType::Event:
                    if (!processEventPackage(std::move(package))) {
                        Debug::warning("Event package processing failed");
                    }
                    break;
                default:
                    Debug::warning("Unknown package type:", package.type());
                }
            });

            if (size == 0) {
                break;
            } else {
                m_incompleatedStatistics.bytesReadPerSecond += size;
            }
        }
    }

    if (m_statisticsTimer.check()) {
        m_statistics = m_incompleatedStatistics;
        m_incompleatedStatistics = Statistics{.bytesWritenPerSecond = 0, .bytesReadPerSecond = 0};
    }
}

std::optional<e172::Event> e172::GameServer::pullEvent()
{
    if (!m_eventQueue.empty()) {
        const auto e = m_eventQueue.front();
        m_eventQueue.pop();
        return e;
    }
    return std::nullopt;
}

void e172::GameServer::entityAdded(const e172::ptr<Entity> &entity)
{
    m_entityAddEventQueue.push(entity);
}

void e172::GameServer::entityRemoved(const Entity::Id &id)
{
    m_entityRemoveEventQueue.push(id);
}

void e172::GameServer::refreshSockets()
{
    auto it = m_clients.begin();
    while (it != m_clients.end()) {
        if (it->socket->isConnected()) {
            ++it;
        } else {
            const auto id = it->id;
            it = m_clients.erase(it);
            m_clientDisconnected(id, Private{});
        }
    }

    while (const auto conn = m_server->pullConnection()) {
        const auto clientId = m_nextClientId++;
        if (conn->isConnected()) {
            m_incompleatedStatistics.bytesWritenPerSecond
                += WritePackage::push(*conn,
                                      PackageType(GamePackageType::Init),
                                      [clientId](WritePackage p) {
                                          p.write<PackedClientId>(clientId);
                                      });
        }
        for (const auto &e : m_app.entities()) {
            sendEntityAddedPackage(*conn, e);
        }
        m_clients.push_back(Client{.id = clientId, .socket = conn});
        m_clientConnected(clientId, Private{});
    }
}

bool e172::GameServer::processEventPackage(ReadPackage &&package)
{
    if (const auto event = Event::deserializeConsume(
            ReadPackage::readAll<ReadBuffer>(std::move(package)))) {
        m_eventQueue.push(*event);
        return true;
    }
    return false;
}

void e172::GameServer::broadcastEntityAddedPackage(const ptr<Entity> &entity)
{
    assert(entity);
    const auto type = entity->meta().typeName();
    const auto id = entity->entityId();

    for (const auto &client : m_clients) {
        if (client.socket->isConnected()) {
            m_incompleatedStatistics.bytesWritenPerSecond
                += WritePackage::push(*client.socket,
                                      PackageType(GamePackageType::AddEntity),
                                      [type, id](WritePackage p) {
                                          p.writeDyn(type);
                                          p.write(PackedEntityId(id));
                                      });
        }
    }
}

void e172::GameServer::sendEntityAddedPackage(Socket &s, const ptr<Entity> &entity)
{
    assert(entity);
    const auto type = entity->meta().typeName();
    const auto id = entity->entityId();
    if (s.isConnected()) {
        m_incompleatedStatistics.bytesWritenPerSecond
            += WritePackage::push(s,
                                  PackageType(GamePackageType::AddEntity),
                                  [type, id](WritePackage p) {
                                      p.writeDyn(type);
                                      p.write(PackedEntityId(id));
                                  });
    }
}
