#pragma once

#include <src/entity.h>
#include <src/math/physicalobject.h>
#include <src/net/common.h>
#include <src/net/netsync.h>

namespace e172::integration {

class Shooter : public e172::Entity, public e172::PhysicalObject
{
public:
    Shooter(e172::FactoryMeta &&meta, e172::PackedClientId id);

    // Entity interface
public:
    virtual void proceed(e172::Context *context, e172::EventHandler *eventHandler) override;
    virtual void render(e172::AbstractRenderer *renderer) override;

private:
    e172::PackedClientId m_id;
};

} // namespace e172::integration
