#include "Enemy.h"

Enemy::Enemy(RenderWindow* window, Vector2f position, Vector2f velocity) :
	GameObject(window, 20.f, 1.f, position, velocity)
{
	spawnTimer = (float)(rand() % 5 + 5);

	if (texture.loadFromFile(TEXTURE_PATH)) {
		shape.setTexture(&texture);
	}

	Vector2f pos;
	switch (rand() % 4) {
	case 0:
		pos.x = 0.f;
		pos.y = (float)(rand() % window->getSize().y);
		break;
	case 1:
		pos.x = (float)window->getSize().x;
		pos.y = (float)(rand() % window->getSize().y);
		break;
	case 2:
		pos.x = (float)(rand() % window->getSize().x);
		pos.y = (float)window->getSize().y;
		break;
	case 3:
		pos.x = (float)(rand() % window->getSize().x);
		pos.y = 0.f;
		break;
	}
	shape.setPosition(pos);
}

void Enemy::Update(float deltaTime) {
	if (spawnTimer > 0) {
		spawnTimer -= deltaTime;
	}
	else if (enabled) {
		if (zagTimer <= 0.f) {
			zagTimer = (rand() % 100) * 0.05f + 1.f;
			float angle = rand() % 360 * PI / 180.f;
			thrust = THRUST * Vector2f(sin(angle), cos(angle));
		}
		else {
			zagTimer -= deltaTime;
		}
		velocity -= velocity * DRAG_COEFF * deltaTime;
		velocity += thrust * deltaTime;
		ApplyVelocity(deltaTime);
	}
}

void Enemy::OnCollide(bool destroysAsteroids, bool destroysShips) {
	if (destroysAsteroids) {
		enabled = false;
	}
}

void Enemy::Draw() const {
	if (spawnTimer <= 0.f && enabled) {
		window->draw(shape);
	}
}
