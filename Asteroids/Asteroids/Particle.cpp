#include "Particle.h"

Particle::Particle() {
	lifeTimer = lifeTime;
}

void Particle::SetActive(Vector2f position, Vector2f velocity, float size, float lifeTime, Texture& texture, Color color) {
	this->lifeTime = lifeTime;
	this->startSize = size;
	lifeTimer = 0.f;
	shape.setPosition(position);
	this->velocity = velocity;
	shape.setTexture(&texture);
	Vector2f vSize = Vector2f(size, size);
	shape.setSize(vSize);
	shape.setOrigin(vSize * 0.5f);
	shape.setFillColor(color);
}

void Particle::Update(float deltaTime) {
	if (IsActive()) {
		lifeTimer += deltaTime;
		Vector2f pos = shape.getPosition();
		pos += velocity * deltaTime;
		shape.setPosition(pos);
		Vector2f size = Vector2f(startSize, startSize) * (lifeTime - lifeTimer) / lifeTime;
		shape.setSize(size);
		shape.setOrigin(size * 0.5f);
	}
}