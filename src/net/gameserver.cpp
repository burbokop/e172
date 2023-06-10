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
        for (const auto &s : m_sockets) {
            WritePackage::push(*s,
                               PackageType(GamePackageType::RemoveEntity),
                               [id](WritePackage p) { p.write(PackedEntityId(id)); });
        }
        m_entityRemoveEventQueue.pop();
    }

    for (const auto &e : m_app.entities()) {
        if (e->needSyncNet()) {
            const auto id = e->entityId();
            WriteBuffer buf;
            e->writeNet(buf);

            for (const auto &s : m_sockets) {
                WritePackage::push(*s,
                                   PackageType(GamePackageType::SyncEntity),
                                   [id, &buf](WritePackage p) {
                                       p.write(PackedEntityId(id));
                                       p.write(std::move(buf));
                                   });
            }
        }
    }

    for (const auto &s : m_sockets) {
        s->flush();
    }

    for (const auto &s : m_sockets) {
        while (auto package = ReadPackage::pull(*s, [this](ReadPackage package) {
                   switch (GamePackageType(package.type())) {
                   case GamePackageType::Event:
                       if (!processEventPackage(std::move(package))) {
                           Debug::warning("Event package processing failed");
                       }
                       break;
                   default:
                       throw UnknownPackageTypeException(package.type());
                   }
               })) {
        }
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
    auto it = m_sockets.begin();
    while (it != m_sockets.end()) {
        if ((*it)->isConnected()) {
            ++it;
        } else {
            it = m_sockets.erase(it);
        }
    }

    while (const auto conn = m_server->pullConnection()) {
        const auto clientId = m_nextClientId++;
        WritePackage::push(*conn, PackageType(GamePackageType::Init), [clientId](WritePackage p) {
            p.write<PackedClientId>(clientId);
        });
        for (const auto &e : m_app.entities()) {
            sendEntityAddedPackage(*conn, e);
        }
        m_sockets.push_back(conn);
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

    for (const auto &s : m_sockets) {
        WritePackage::push(*s, PackageType(GamePackageType::AddEntity), [type, id](WritePackage p) {
            p.writeDyn(type);
            p.write(PackedEntityId(id));
        });
    }
}

void e172::GameServer::sendEntityAddedPackage(Socket &s, const ptr<Entity> &entity)
{
    assert(entity);
    const auto type = entity->meta().typeName();
    const auto id = entity->entityId();

    WritePackage::push(s, PackageType(GamePackageType::AddEntity), [type, id](WritePackage p) {
        p.writeDyn(type);
        p.write(PackedEntityId(id));
    });
}
