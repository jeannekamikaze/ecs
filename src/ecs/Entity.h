#pragma once

#include <cstddef>

namespace ecs
{
// An Entity is essentially a size_t value.
// We newtype int to hide the representation from the user
// and to protect any manipulation of the underlying int value.
class Entity
{
	template <typename Component>
	friend class ComponentManager;

	template <typename... Components>
	friend class Manager;

	std::size_t id;

	Entity(std::size_t id) : id{ id } {}

public:
	operator std::size_t() const { return id; }
};

} // namespace ecs