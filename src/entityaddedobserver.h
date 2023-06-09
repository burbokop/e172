#pragma once

#include "utility/ptr.h"

namespace e172 {

class GameApplication;
class Entity;

class EntityAddedObserver
{
    friend GameApplication;

public:
    ~EntityAddedObserver() = default;

protected:
    virtual void entityAdded(const ptr<Entity> &) = 0;
};

} // namespace e172
