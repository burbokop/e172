// Copyright 2023 Borys Boiko

#include "bullet.h"

#include "../../../src/context.h"
#include <utility>

namespace e172::integration {

Bullet::Bullet(e172::FactoryMeta &&meta)
    : e172::Entity(std::move(meta))
{}

void Bullet::proceed(e172::Context *context, e172::EventHandler *eventHandler)
{
    if (m_lifeTimer.check()) {
        context->emitMessage(e172::Context::DestroyEntity, entityId());
    }
    proceedPhysics(context->deltaTime());
}

} // namespace e172::integration
