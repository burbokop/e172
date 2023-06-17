#include "bullet.h"

#include <src/context.h>
#include <src/graphics/abstractrenderer.h>

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

void Bullet::render(e172::AbstractRenderer *renderer) {}

} // namespace e172::integration
