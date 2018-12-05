#include "Bullet.h"

Bullet::Bullet(RenderWindow* window, Vector2f position, Vector2f velocity, bool destroysAsteroids, bool destroysShips) :
	GameObject(window, 2.f, 0.1f, position, velocity) {
	this->destroysAsteroids = destroysAsteroids;
	this->destroysShips = destroysShips;
}

void Bullet::Update(float deltaTime) {
	lifeTimer += deltaTime;
	ApplyVelocity(deltaTime);
}

void Bullet::OnCollide(bool destroysAsteroids, bool destroysShips) {
	lifeTimer = LIFETIME;
}