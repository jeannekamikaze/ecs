#pragma once

#include "Entity.h"
#include "Messenger.h"

namespace ecs
{

template <typename T, typename... Base>
class Component : public Base...
{
    Entity e;
    Messenger& m;

protected:
    
    Entity owner() const { return e; }

    Messenger& messenger() const { return m; }

public:

    Component(Entity e, Messenger& m) : e{ e }, m{ m } {}

    virtual void operator() (double t, float dt) = 0;
};

} // namespace ecs