#include "shooter.h"
#include "bullet.h"

#include <src/graphics/abstractrenderer.h>

#include <src/context.h>
#include <src/eventhandler.h>

namespace e172::integration {

Shooter::Shooter(e172::FactoryMeta &&meta, e172::PackedClientId id)
    : e172::Entity(std::move(meta))
    , m_id(id)
{}

void Shooter::proceed(e172::Context *context, e172::EventHandler *eventHandler)
{
    if (auto c = eventHandler->client(m_id)) {
        e172::Vector<double> v;
        if (c->keyHolded(e172::ScancodeW)) {
            --v.y();
        } else if (c->keyHolded(e172::ScancodeS)) {
            ++v.y();
        }
        if (c->keyHolded(e172::ScancodeA)) {
            --v.x();
        } else if (c->keyHolded(e172::ScancodeD)) {
            ++v.x();
        }
        addForce(v * 100);

        if (c->keySinglePressed(e172::ScancodeSpace)) {
            const auto velocity = (c->mousePosition().into<double>() - position()).normalized()
                                  * 100;
            const auto bullet = e172::FactoryMeta::make<Bullet>();
            bullet->setFriction(0.15);
            bullet->resetPhysicsProperties(position(), 0, velocity);
            context->addEntity(bullet);
        }
    }

    proceedPhysics(context->deltaTime());
}

void Shooter::render(e172::AbstractRenderer *renderer) {}

} // namespace e172::integration
