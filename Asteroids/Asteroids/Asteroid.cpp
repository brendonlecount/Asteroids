#include "Asteroid.h"

Asteroid::Asteroid(RenderWindow* window, float radius, float mass, Vector2f position, Vector2f velocity, AsteroidSpawn asteroidsSpawned, Texture& texture) :
GameObject(window, radius, mass, position, velocity)
{
	this->asteroidsSpawned = asteroidsSpawned;
	shape.setTexture(&texture);
	shape.setTextureRect(IntRect(0, 0, (int)shape.getRadius() * 2, (int)shape.getRadius() * 2));
}

void Asteroid::Update(float deltaTime) {
	ApplyVelocity(deltaTime);
}

void Asteroid::OnCollide(bool destroysAsteroids, bool destroysShips) {
	if (destroysAsteroids) {
		isDestroyed = true;
	}
}
