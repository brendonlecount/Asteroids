#include "Bullet.h"

Bullet::Bullet(RenderWindow* window, Vector2f position, Vector2f velocity) :
	GameObject(window, 2.f, 0.1f, position, velocity) {

}

void Bullet::Update(float deltaTime) {
	lifeTimer += deltaTime;
	ApplyVelocity(deltaTime);
}

void Bullet::OnCollide(bool destroysAsteroids) {
	lifeTimer = LIFETIME;
}