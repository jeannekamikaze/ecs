#include <ecs/Component.h>
#include <ecs/Manager.h>
#include <ecs/event.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#ifdef WIN32
#include <Windows.h>
#endif

struct Position
{
	float x{ 0 };
	float y{ 0 };

	Position() = default;

	Position(float x, float y) : x{ x }, y{ y } {}
};

Position operator+ (Position a, Position b)
{
	return Position{ a.x + b.x, a.y + b.y };
}

class Graphics : public ecs::Component<Graphics>
{
public:
	Graphics(ecs::Entity e) : Component(e) {}
};

class Physics : public ecs::Component<Physics>
{
public:
	Physics(ecs::Entity e) : Component(e) {}
};

struct CircleGraphics : public Graphics, public ecs::Receive<Position>
{
	const float radius{ 50.0f };
	sf::CircleShape circle;
	sf::RenderWindow& window;

public:

	CircleGraphics(ecs::Entity e, sf::RenderWindow& window)
		: Graphics(e), window{ window }
	{
		circle.setRadius(radius);
		circle.setFillColor(sf::Color(20, 200, 30));
	}

	void operator()(double t, float dt)
	{
		window.draw(circle);
	}

	void onEvent(ecs::Entity src, const Position& p)
	{
		circle.setPosition(p.x - radius, p.y - radius);
	}
};

struct CircularMotion : public Physics, public ecs::Emit<Position>
{
	const float speed{ 3.0f };
	const float radius{ 100.0f };
	const Position center{ 400.0f, 300.0f };

public:

	CircularMotion(ecs::Entity e) : Physics(e) {}

	void operator()(double t, float dt)
	{
		Position p(-radius * (float)cos(t*speed), radius * (float)sin(t*speed));
		emit(owner(), center + p);
	}
};

struct OscillateX : public Physics, public ecs::Emit<Position>
{
	const float speed{ 5.0f };
	const float radius{ 150.0f };
	const Position center{ 400.0f, 100.0f };

public:

	OscillateX(ecs::Entity e) : Physics(e) {}

	void operator()(double t, float dt)
	{
		emit(owner(), center + Position(radius * (float)sin(t*speed), 0.0f));
	}
};

using EManager = ecs::Manager<Graphics, Physics>;

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main ()
#endif
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game");
	window.setFramerateLimit(60);
	
    EManager manager;
    ecs::Entity e1 = manager.createEntity();
	auto graphics = new CircleGraphics(e1, window);
	auto physics = new CircularMotion(e1);
	physics->emit += graphics;
    manager.component<Graphics>::set(e1, graphics);
    manager.component<Physics>::set(e1, physics);

	ecs::Entity e2 = manager.createEntity();
	auto graphics2 = new CircleGraphics(e2, window);
	auto physics2 = new OscillateX(e2);
	physics2->emit += graphics2;
	manager.component<Graphics>::set(e2, graphics2);
	manager.component<Physics>::set(e2, physics2);

	sf::Clock clock;
	clock.restart();
	sf::Time t0 = clock.getElapsedTime();
	while (window.isOpen())
	{
		sf::Time t1 = clock.getElapsedTime();
		float elapsed = (t1 - t0).asSeconds();
		t0 = t1;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}
		double time = clock.getElapsedTime().asSeconds();
		window.clear(sf::Color(220, 240, 230));
		manager.component<Physics>::step(time, elapsed);
		manager.component<Graphics>::step(time, elapsed);
		window.display();
    }

	return 0;
}
