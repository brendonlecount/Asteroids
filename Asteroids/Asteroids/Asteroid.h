#pragma once

#include "GameObject.h"

class Asteroid : public GameObject {
	bool isDestroyed = false;
	AsteroidSpawn asteroidsSpawned;

public:
	Asteroid(RenderWindow* window, float radius, float mass, Vector2f position, Vector2f velocity, AsteroidSpawn asteroidsSpawned);
	virtual void Update(float deltaTime);
	virtual void OnCollide(bool destroysAsteroids);
	virtual bool CollisionEnabled() const { return true; }
	virtual bool IsStatic() const { return false; }
	virtual bool IsDestroyed() const { return isDestroyed; }
	virtual bool IsAsteroid() const { return true; }
	virtual bool IsShip() const { return false; }
	virtual AsteroidSpawn GetAsteroidsSpawned() const { return asteroidsSpawned; }
	virtual bool DestroysAsteroids() const { return false; }
};