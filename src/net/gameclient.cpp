#include "gameclient.h"

#include "../abstracteventprovider.h"
#include "../entity.h"
#include "../gameapplication.h"
#include "common.h"
#include "networker.h"
#include "src/context.h"
#include "src/debug.h"
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
                   case GamePackageType::AddEntity:
                       if (!processAddEntityPackage(std::move(package))) {
                           Debug::warning("AddEntity package processing failed");
                       }
                       break;
                   case GamePackageType::RemoveEntity:
                       if (!processRemoveEntityPackage(std::move(package))) {
                           Debug::warning("RemoveEntity package processing failed");
                       }
                       break;
                   case GamePackageType::SyncEntity:
                       if (!processSyncEntityPackage(std::move(package))) {
                           Debug::warning("SyncEntity package processing failed");
                       }
                       break;
                   default:
                       throw UnknownPackageTypeException(package.type());
                   }
               })) {
        }
    }
}

bool e172::GameClient::processAddEntityPackage(ReadPackage &&package)
{
    assert(m_networker);
    const auto type = package.readDyn<std::string>();
    if (!type)
        return false;
    const auto id = package.read<PackedEntityId>();
    if (!id)
        return false;

    if (const auto &e = m_networker->m_entityFactory.create(*type)) {
        e->m_entityId = *id;
        m_app.addEntity(e);
    } else {
        Debug::warning("GameClient::processAddEntityPackage: unknown entity type:", *type);
        return false;
    }
    return true;
}

bool e172::GameClient::processRemoveEntityPackage(ReadPackage &&package)
{
    assert(m_app.context());
    const auto id = package.read<PackedEntityId>();
    if (!id)
        return false;
    m_app.context()->emitMessage(e172::Context::DESTROY_ENTITY, *id);
    return true;
}

bool e172::GameClient::processSyncEntityPackage(ReadPackage &&package)
{
    const auto id = package.read<PackedEntityId>();
    if (!id)
        return false;

    if (const auto entity = m_app.entityById(*id)) {
        if (!entity->readNet(ReadPackage::readAll<ReadBuffer>(std::move(package)))) {
            return false;
        }
    }
    return true;
}
