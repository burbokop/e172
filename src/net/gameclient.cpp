#include "gameclient.h"

#include "../abstracteventprovider.h"
#include "../entity.h"
#include "../gameapplication.h"
#include "common.h"
#include "src/utility/package.h"

void e172::GameClient::sync()
{
    if (m_socket) {
        if (!m_socket->isConnected()) {
            m_socket = nullptr;
            return;
        }

        auto eventProvider = m_app.eventProvider();
        assert(eventProvider);

        while (auto event = eventProvider->pullEvent()) {
            if (event->type() == Event::Quit) {
                m_app.quitLater();
                return;
            }

            WriteBuffer buf;
            buf.write(*event);
            WritePackage::push(*m_socket,
                               PackageType(GamePackageType::Event),
                               [&buf](WritePackage p) {
                                   p.write<PackedPlayerId>(0 /*TODO*/);
                                   p.write(std::move(buf));
                               });
        }
        m_socket->flush();

        while (auto package = ReadPackage::pull(*m_socket, [this](ReadPackage package) {
                   switch (GamePackageType(package.type())) {
                   case GamePackageType::SyncEntity:
                       syncEntity(std::move(package));
                       break;
                   default:
                       throw UnknownPackageTypeException(package.type());
                   }
               })) {
        }
    }
}

void e172::GameClient::syncEntity(ReadPackage &&package)
{
    const Entity::id_t id = package.read<PackedEntityId>().value();
    if (const auto entity = m_app.entityById(id)) {
        entity->readNet(ReadPackage::readAll<ReadBuffer>(std::move(package)));
    }
}
