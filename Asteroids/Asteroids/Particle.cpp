#include "Particle.h"

Particle::Particle() {
	lifeTimer = LIFETIME;
}

void Particle::SetActive(Vector2f position, Vector2f velocity) {
	lifeTimer = 0.f;
	shape.setPosition(position);
	this->velocity = velocity;
}
void Particle::Update(float deltaTime) {
	Vector2f pos = shape.getPosition();
	pos += velocity * deltaTime;
	shape.setPosition(pos);
}