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

template <class Message>
class Emitter
{
public:
	event<Entity, Message> emit;
};

template <class Message>
class Receiver
{
public:
	virtual void onEvent(Entity src, const Message&) = 0;
};

template <typename X, typename... XS>
class Emit : public Emit<X>, public Emit<XS...> {};

template <typename X>
class Emit<X> : public Emitter<X>{};

template <typename X, typename... XS>
class Receive : public Receive<X>, public Receive<XS...> {};

template <typename X>
class Receive<X> : public Receiver<X>{};

} // namespace OGDT
