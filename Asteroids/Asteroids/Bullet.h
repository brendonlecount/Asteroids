#pragma once

#include "GameObject.h"

class Bullet : public GameObject {
	const float LIFETIME = 2.f;
	float lifeTimer = 0.f;

public:
	Bullet(RenderWindow* window, Vector2f position, Vector2f velocity);
	virtual void Update(float deltaTime);
	virtual void OnCollide(bool destroysAsteroids);
	virtual bool CollisionEnabled() const { return true; }
	virtual bool IsStatic() const { return false; }
	virtual bool IsDestroyed() const { return lifeTimer >= LIFETIME; }
	virtual bool IsAsteroid() const { return false; }
	virtual bool IsShip() const { return false; }
	virtual AsteroidSpawn GetAsteroidsSpawned() const { return AsteroidSpawn::None; }
	virtual bool DestroysAsteroids() const { return true; }
};