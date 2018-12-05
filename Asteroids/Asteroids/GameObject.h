#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <vector>

using namespace std;
using namespace sf;

class GameObject {
protected:
	CircleShape shape;
	Texture texture;
	RenderWindow* window;
	Vector2f velocity;
	float mass;

	void ApplyVelocity(float deltaTime);

public:
	enum AsteroidSpawn { None, Medium, Small };

	GameObject(RenderWindow* window, float radius, float mass, Vector2f position, Vector2f velocity);
	virtual void Update(float deltaTime) = 0;
	virtual void OnCollide(bool destroysAsteroids, bool destroysShips) = 0;
	virtual bool CollisionEnabled() const = 0;
	virtual bool IsStatic() const = 0;
	virtual bool DestroysAsteroids() const = 0;
	virtual bool DestroysShips() const = 0;
	virtual bool IsDestroyed() const = 0;
	virtual bool IsAsteroid() const = 0;
	virtual bool IsShip() const = 0;
	virtual AsteroidSpawn GetAsteroidsSpawned() const = 0;
	float GetRadius() const { return shape.getRadius(); }
	float GetMass() const { return mass; }
	Vector2f GetPosition() const { return shape.getPosition(); }
	Vector2f GetVelocity() const { return velocity; }
	void SetVelocity(Vector2f velocity) { this->velocity = velocity; }
	virtual void Draw() const { window->draw(shape); }
};