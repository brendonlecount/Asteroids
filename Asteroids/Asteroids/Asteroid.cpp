#include "Asteroid.h"

Asteroid::Asteroid(RenderWindow* window, float radius, float mass, Vector2f position, Vector2f velocity, AsteroidSpawn asteroidsSpawned) :
GameObject(window, radius, mass, position, velocity)
{
	this->asteroidsSpawned = asteroidsSpawned;
}

void Asteroid::Update(float deltaTime) {
	ApplyVelocity(deltaTime);
}

void Asteroid::OnCollide(bool destroysAsteroids) {
	if (destroysAsteroids) {
		isDestroyed = true;
	}
}
