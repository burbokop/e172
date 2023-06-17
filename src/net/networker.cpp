// Copyright 2023 Borys Boiko

#include "networker.h"

#include "../gameapplication.h"

e172::Either<e172::Networker::Error, std::shared_ptr<e172::GameServer>> e172::Networker::listen(
    GameApplication &app, uint16_t port)
{
    return listen(port).map<std::shared_ptr<GameServer>>(
        [&app, this](const std::shared_ptr<Server> &s) {
            const auto result = std::make_shared<GameServer>(app, this, s, GameServer::Private{});
            app.observeEntityLifeTime(result);
            return result;
        });
}
