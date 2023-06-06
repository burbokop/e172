#include "networker.h"

#include "../gameapplication.h"

#include "../utility/buffer.h"

#include <src/utility/package.h>

namespace e172 {

using PackedEntityId = std::uint64_t;

class UnknownPackageTypeException : public std::exception
{
public:
    UnknownPackageTypeException(PackageType type)
        : m_message(std::string("Unknown package type: ") + std::to_string(type)){};

    // exception interface
public:
    const char *what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

namespace packageType {
namespace {

constexpr std::uint8_t SyncEntity = 0;

}
} // namespace packageType
} // namespace e172

void e172::GameClient::sync()
{
    if (m_socket) {
        if (!m_socket->isConnected()) {
            m_socket = nullptr;
            return;
        }
        for (const auto &e : m_app.entities()) {
            if (e->anyNetSyncDirty()) {
                const auto id = e->entityId();
                const auto bytes = e->collectBytes();

                WritePackage::pack(*m_socket, packageType::SyncEntity, [id, &bytes](WritePackage p) {
                    p.write(PackedEntityId(id));
                    p.write(bytes.data(), bytes.size());
                });
            }
        }
    }
}

void e172::GameServer::sync()
{
    refreshSockets();
    for (const auto &s : m_sockets) {
        while (auto package = ReadPackage::pull(*s, [this](ReadPackage package) {
                   switch (package.type()) {
                   case packageType::SyncEntity:
                       syncEntity(std::move(package));
                   default:
                       throw UnknownPackageTypeException(package.type());
                   }
               })) {
        }
    }

    todo;
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

void e172::GameServer::syncEntity(ReadPackage &&package)
{
    const Entity::id_t id = package.read<PackedEntityId>().value();
    if (const auto entity = m_app.entityById(id)) {
        entity->assignBytes(ReadPackage::readAll(std::move(package)), true);
    }
}
