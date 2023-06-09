#pragma once

#include "gameclient.h"
#include "gameserver.h"
#include <list>
#include <src/memcontrol/abstractfactory.h>
#include <src/meta.h>
#include <src/utility/either.h>
#include <vector>

namespace e172 {

class Networker
{
    friend GameClient;

public:
    enum Error {
        FailedToCreateSocket,
        ConnectionRefused,
        UnwnownConnectionError,
        AddressAlreadyInUse,
        UnwnownBindingError,
        ListenFailed
    };

    static constexpr const char *localhost = "127.0.0.1";

    Networker() = default;

    friend std::ostream &operator<<(std::ostream &stream, Error e)
    {
        switch (e) {
        case Networker::FailedToCreateSocket:
            return stream << "FailedToCreateSocket";
        case Networker::ConnectionRefused:
            return stream << "ConnectionRefused";
        case Networker::UnwnownConnectionError:
            return stream << "UnwnownConnectionError";
        case Networker::AddressAlreadyInUse:
            return stream << "AddressAlreadyInUse";
        case Networker::UnwnownBindingError:
            return stream << "UnwnownBindingError";
        case Networker::ListenFailed:
            return stream << "ListenFailed";
        }
        return stream << "UnknownError";
    }

    Either<Error, std::shared_ptr<GameServer>> listen(GameApplication &app, std::uint16_t port);

    Either<Error, std::shared_ptr<GameClient>> connect(GameApplication &app,
                                                       std::uint16_t port,
                                                       const std::string &address = localhost)
    {
        return connect(port, address).map<std::shared_ptr<GameClient>>([&app, this](auto s) {
            return std::make_shared<GameClient>(app, this, s, GameClient::Private{});
        });
    }

    template<typename T, typename... Args>
    std::string registerEntityType(Args &&...args)
        requires std::is_base_of<Entity, T>::value
    {
        const auto tn = Meta::fromType<T>().typeName();
        return m_entityFactory.registerType<T>(tn, [args...]() -> Entity * {
            return FactoryMeta::make<T>(args...);
        });
        return tn;
    }

    virtual Either<Error, std::shared_ptr<Server>> listen(std::uint16_t port) = 0;
    virtual Either<Error, std::shared_ptr<Socket>> connect(std::uint16_t port,
                                                           const std::string &address = localhost)
        = 0;

    virtual ~Networker() = default;

private:
    AbstractFactory<std::string, Entity> m_entityFactory;
};

} // namespace e172
