#pragma once

#include <e172/src/entity.h>
#include <src/math/physicalobject.h>
#include <src/time/elapsedtimer.h>

namespace e172::integration {

class Bullet : public e172::Entity, public e172::PhysicalObject
{
public:
    Bullet(e172::FactoryMeta &&meta);

    // Entity interface
public:
    void proceed(e172::Context *context, e172::EventHandler *eventHandler) override;
    void render(e172::AbstractRenderer *renderer) override;

private:
    e172::ElapsedTimer m_lifeTimer = 10000;
};

} // namespace e172::integration
