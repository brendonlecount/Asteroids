#include "Explosion.h"

Explosion::Explosion(RenderWindow* window, Vector2f position, Vector2f velocity, float scale) {
	this->window = window;
	particleTexture.loadFromFile(PARTICLE_TEXTURE_PATH);
	Color color = Color(100, 100, 100, 100);
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		float angle = (float)(rand() % 360) * 3.14149f / 180.f;
		Vector2f v = scale * 200.f * Vector2f(cos(angle), sin(angle));
		particles[i].SetActive(position, velocity + v, scale * 16.f, 0.5f, particleTexture, color);
	}
}

void Explosion::Update(float deltaTime) {
	lifeTimer += deltaTime;
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		particles[i].Update(deltaTime);
	}
}

void Explosion::Draw() const {
	for (int i = 0; i < PARTICLE_COUNT; i++) {
		window->draw(particles[i].GetShape());
	}
}
