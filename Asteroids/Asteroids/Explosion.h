#pragma once

#include "Particle.h"

class Explosion {
	RenderWindow* window;
	Particle particles[10];

	Texture particleTexture;
	float lifeTimer = 0.f;

	const string PARTICLE_TEXTURE_PATH = "Assets/whitelight.png";
	const int PARTICLE_COUNT = 10;
	const float LIFETIME = 0.5f;

public:
	Explosion(RenderWindow* window, Vector2f position, Vector2f velocity, float scale);
	void Update(float deltaTime);
	void Draw() const;
	bool GetActive() const { return lifeTimer < LIFETIME; }
};