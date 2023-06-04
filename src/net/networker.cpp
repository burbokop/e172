#include "networker.h"

#include "../gameapplication.h"

#include "../utility/buffer.h"

namespace e172::packageType {

namespace {

constexpr std::uint8_t SyncEntity = 0;

} // namespace

} // namespace e172::packageType

void e172::GameClient::sync()
{
    for (const auto &e : m_app.entities()) {
        if (e->anyNetSyncDirty()) {
            const auto id = e->entityId();
            const auto bytes = e->collectBytes();
            m_socket->write([id, &bytes](WritePackage p) {
                p.write(packageType::SyncEntity);
                p.write(std::uint64_t(id));
                p.write(bytes.data(), bytes.size());
            });
        }
    }
}

void e172::GameServer::sync()
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

    todo;
}
