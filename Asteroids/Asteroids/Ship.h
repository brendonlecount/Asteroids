#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "GameObject.h"

class Ship : public GameObject {
	float deathTimer;
	float deathBlinkTimer = 0.f;
	bool isPlayer2 = false;
	int lives;

	const float DEATH_TIME = 2.f;
	const float DEATH_BLINK_TIME = 0.1f;
	const float ROTATION_RATE = 180.f;
	const float DRAG_COEFF = 1.1f;
	const float THRUST = 400.f;
	const float PI = 3.14159f;

public:
	Ship(RenderWindow* window, Vector2f position, Vector2f velocity, int lives, bool isPlayer2);
	float GetAngle() const { return -shape.getRotation() * PI / 180.f; }
	virtual void Update(float deltaTime);
	virtual void OnCollide(bool destroysAsteroids, bool destroysShips);
	virtual bool CollisionEnabled() const;
	virtual bool IsStatic() const { return false; }
	virtual bool IsDestroyed() const { return false; }
	virtual bool IsAsteroid() const { return false; }
	virtual bool IsShip() const { return true; }
	virtual AsteroidSpawn GetAsteroidsSpawned() const { return AsteroidSpawn::None; }
	virtual bool DestroysAsteroids() const { return true; }
	virtual bool DestroysShips() const { return true; }
	virtual void Draw() const;
	int GetLives() { return lives; }
};