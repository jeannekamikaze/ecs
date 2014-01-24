#pragma once

#include "Entity.h"
#include <list>
#include <functional>

namespace ecs
{

template <class... Args>
class event
{
    typedef std::function<void(Args...)> callback;
    std::list<callback> callbacks;

public:
    
    void operator+= (const callback& c)
    {
        callbacks.push_back(c);
    }

    template <typename T>
    void operator+= (T& t)
    {
        callbacks.push_back([&t](Args... args) { t.onEvent(args...); });
    }

    template <typename T>
    void operator+= (T* t)
    {
        callbacks.push_back([=](Args... args) { t->onEvent(args...); });
    }

    void operator() (Args... args)
    {
        for (const callback& c : callbacks) c(args...);
    }
};

template <typename... Messages>
class _Emitter
{
public:
    event<Entity, Messages...> emit;
};

template <typename Message>
class _Receiver
{
public:
    virtual void onEvent(Entity src, const Message&) = 0;
};

template <typename X, typename... XS>
class Emitter : public Emitter<X>, public Emitter<XS...> {};

template <typename X>
class Emitter<X> : public _Emitter<X>{};

template <typename X, typename... XS>
class Receiver : public Receiver<X>, public Receiver<XS...> {};

template <typename X>
class Receiver<X> : public _Receiver<X>{};

} // namespace OGDT
