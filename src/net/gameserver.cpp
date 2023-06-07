#include "gameserver.h"
#include "../gameapplication.h"
#include "common.h"
#include "src/utility/package.h"

void e172::GameServer::sync()
{
    refreshSockets();

    for (const auto &e : m_app.entities()) {
        if (e->anyNetSyncDirty()) {
            const auto id = e->entityId();
            WriteBuffer buf;
            e->writeNet(buf);

            for (const auto &s : m_sockets) {
                WritePackage::push(*s,
                                   PackageType(GamePackageType::SyncEntity),
                                   [id, &buf](WritePackage p) {
                                       p.write(PackedEntityId(id));
                                       p.writeBuf(std::move(buf));
                                   });
            }
        }
    }

    for (const auto &s : m_sockets) {
        while (auto package = ReadPackage::pull(*s, [this](ReadPackage package) {
                   switch (GamePackageType(package.type())) {
                   case GamePackageType::Event:
                       processEventPackage(std::move(package));
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
        m_sockets.push_back(conn);
    }
}

void e172::GameServer::processEventPackage(ReadPackage &&package)
{
    const auto playerId = package.read<PackedPlayerId>();
    m_eventQueue.push(Event::readNet(ReadPackage::readAll<ReadBuffer>(std::move(package))));
}
