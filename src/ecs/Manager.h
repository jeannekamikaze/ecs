#pragma once

#include "Entity.h"
#include <vector>

namespace ecs
{

// Managers will be using this type of container to manage
// components and entities.
template <typename T>
using container = std::vector<T>;

// A simple Manager for a specific type of Component.
template <typename Component>
class ComponentManager
{
    container<Component*> components;

    ComponentManager(const ComponentManager&)  = delete;
    ComponentManager(const ComponentManager&&) = delete;
    ComponentManager& operator= (const ComponentManager&)  = delete;
    ComponentManager& operator= (const ComponentManager&&) = delete;

public:

    ComponentManager() = default;

    ~ComponentManager()
    {
        for (Component* c : components) if (c) delete c;
    }

    void set(Entity e, Component* c)
    {
        components.resize(e.id + 1);
        components[e.id] = c;
    }

    void remove(Entity e)
    {
        delete components[e.id];
        components[e.id] = nullptr;
    }

    void step(double t, float dt)
    {
        for (Component* c : components) if (c) (*c)(t, dt);
    }
};

// A Manager that is an aggregate of simple Component Managers.
// This Manager also manages Entities and provides a centralised
// location for their creation and destruction.
template <typename Component, typename... Components>
class ComponentsManager : public ComponentsManager<Component>, public ComponentsManager<Components...> {};

template <typename Component>
class ComponentsManager<Component> : public ComponentManager<Component> {};

template <typename... Components>
class Manager : public ComponentsManager<Components...>
{
    container<bool> entities;
    container<Entity> free_entities;

    Manager(const Manager&)  = delete;
    Manager(const Manager&&) = delete;
    Manager& operator= (const Manager&)  = delete;
    Manager& operator= (const Manager&&) = delete;

public:

    template <typename C>
    using component = ComponentManager<C>;

    Manager() = default;
    
    Entity createEntity()
    {
        std::size_t id;
        if (!free_entities.empty())
        {
            id = free_entities.back();
            free_entities.pop_back();
        }
        else
        {
            id = entities.size();
            entities.resize(id + 1);
        }
        entities[id] = true;
        return id;
    }

    void destroyEntity(Entity e)
    {
        entities[e] = false;
        free_entities.push_back(e);
    }
};

} // namespace ecs
