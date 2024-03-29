// Copyright 2023 Borys Boiko

#include "gameserver.h"

#include "../assettools/loadable.h"
#include "../debug.h"
#include "../gameapplication.h"
#include "../utility/package.h"
#include "common.h"
#include "networker.h"
#include <utility>

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
    if (refreshSockets() == 0)
        return;

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

std::size_t e172::GameServer::sendCustomPackage(PackedClientId clientId,
                                                PackageType type,
                                                const std::function<void(WritePackage)> &writeFn)
{
    const auto it = std::find_if(m_clients.begin(), m_clients.end(), [clientId](const auto &c) {
        return c.id == clientId;
    });
    if (it != m_clients.end()) {
        assert(it->socket);
        if (it->socket) {
            return WritePackage::push(*it->socket, type, writeFn);
        }
    }
    return 0;
}

void e172::GameServer::broadcastCustomPackage(PackageType type,
                                              const std::function<void(WritePackage)> &writeFn)
{
    (void) type;
    (void) writeFn;
    todo();
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

std::size_t e172::GameServer::refreshSockets()
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
    return m_clients.size();
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
    assert(entity.data());
    if (!entity)
        return;

    const auto id = entity->entityId();
    if (const auto loadable = smart_cast<Loadable>(entity)) {
        const auto templateId = loadable->templateId();
        for (const auto &client : m_clients) {
            if (client.socket->isConnected()) {
                m_incompleatedStatistics.bytesWritenPerSecond
                    += WritePackage::push(*client.socket,
                                          PackageType(GamePackageType::AddLoadableEntity),
                                          [&templateId, id](WritePackage p) {
                                              p.writeDyn(templateId);
                                              p.write(PackedEntityId(id));
                                          });
            }
        }
    } else {
        const auto type = entity->meta().typeName();
        for (const auto &client : m_clients) {
            if (client.socket->isConnected()) {
                m_incompleatedStatistics.bytesWritenPerSecond
                    += WritePackage::push(*client.socket,
                                          PackageType(GamePackageType::AddEntity),
                                          [&type, id](WritePackage p) {
                                              p.writeDyn(type);
                                              p.write(PackedEntityId(id));
                                          });
            }
        }
    }
}

void e172::GameServer::sendEntityAddedPackage(Socket &s, const ptr<Entity> &entity)
{
    assert(entity);
    if (s.isConnected() && entity) {
        const auto id = entity->entityId();
        if (const auto loadable = smart_cast<Loadable>(entity)) {
            const auto templateId = loadable->templateId();
            m_incompleatedStatistics.bytesWritenPerSecond
                += WritePackage::push(s,
                                      PackageType(GamePackageType::AddLoadableEntity),
                                      [&templateId, id](WritePackage p) {
                                          p.writeDyn(templateId);
                                          p.write(PackedEntityId(id));
                                      });
        } else {
            const auto type = entity->meta().typeName();
            m_incompleatedStatistics.bytesWritenPerSecond
                += WritePackage::push(s,
                                      PackageType(GamePackageType::AddEntity),
                                      [&type, id](WritePackage p) {
                                          p.writeDyn(type);
                                          p.write(PackedEntityId(id));
                                      });
        }
    }
}
