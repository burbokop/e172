#pragma once

#include "entity.h"
#include "utility/ptr.h"

namespace e172 {

class GameApplication;

class EntityLifeTimeObserver
{
    friend GameApplication;

public:
    ~EntityLifeTimeObserver() = default;

protected:
    virtual void entityAdded(const ptr<Entity> &) = 0;
    virtual void entityRemoved(const Entity::Id &) = 0;
};

} // namespace e172
