#pragma once

#include "Entity.h"

namespace ecs
{

template <typename T, typename... Base>
class Component : public Base...
{
	Entity e;

protected:

	Entity owner() const { return e; }

public:

	Component(Entity e) : e{ e } {}

	virtual void operator() (double t, float dt) = 0;
};

} // namespace ecs