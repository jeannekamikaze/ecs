#pragma once

#include "Entity.h"
#include "Event.h"
#include <typeindex>
#include <unordered_map>
#include <map>
#include <list>
#include <string>

namespace ecs
{

class Messenger
{
    using Listener = Receiver<void*>;

    using MessageMap = std::unordered_map<std::type_index, std::list<Listener*>>;

    using EntityMap = std::map<Entity, MessageMap>;

    EntityMap entity_map;

    Messenger(const Messenger&)  = delete;
    Messenger(const Messenger&&) = delete;
    Messenger& operator= (const Messenger&)  = delete;
    Messenger& operator= (const Messenger&&) = delete;

public:

    Messenger() = default;
    
    template <typename Message>
    void connect(Entity src, Receiver<Message>* receiver)
    {
        auto* l = reinterpret_cast<Listener*>(receiver);
        entity_map[src][std::type_index{ typeid(Message) }].push_back(l);
    }

    template <typename Message>
    void disconnect(Entity src, Receiver<Message>* receiver)
    {
        auto* l = reinterpret_cast<Listener*>(receiver);
        entity_map[e][std::type_index{ typeid(Messages) }].remove(l);
    }

    template <typename Message>
    void send(Entity src, const Message& msg) const
    {
        EntityMap::const_iterator map_it = entity_map.find(src);
        if (map_it != entity_map.end())
        {
            MessageMap::const_iterator listeners_it = map_it->second.find(std::type_index{ typeid(Message) });
            if (listeners_it != map_it->second.end())
            {
                std::list<Listener*> listeners = listeners_it->second;
                for (Listener* l : listeners)
                {
                    auto receiver = reinterpret_cast<Receiver<Message>*>(l);
                    receiver->onEvent(src, msg);
                }
            }
        }
    }
};

} // namespace ecs