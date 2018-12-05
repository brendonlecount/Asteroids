#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "GameObject.h"

class Enemy : public GameObject {
	const float ROTATION_RATE = 180.f;
	const float DRAG_COEFF = 1.1f;
	const float THRUST = 150.f;
	const float PI = 3.14159f;

	bool enabled = true;
	float spawnTimer;
	float zagTimer = 0.f;
	Vector2f thrust;

	const string TEXTURE_PATH = "Assets/enemy.jpg";

public:
	Enemy(RenderWindow* window, Vector2f position, Vector2f velocity);
	virtual void Update(float deltaTime);
	virtual void OnCollide(bool destroysAsteroids, bool destroysShips);
	virtual bool CollisionEnabled() const { return enabled && spawnTimer <= 0.f; }
	virtual bool IsStatic() const { return false; }
	virtual bool IsDestroyed() const { return false; }
	virtual bool IsAsteroid() const { return false; }
	virtual bool IsShip() const { return false; }
	virtual AsteroidSpawn GetAsteroidsSpawned() const { return AsteroidSpawn::None; }
	virtual bool DestroysAsteroids() const { return false; }
	virtual bool DestroysShips() const { return true; }
	virtual void Draw() const;
};