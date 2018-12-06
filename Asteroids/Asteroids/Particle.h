#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace std;
using namespace sf;

class Particle {
	RectangleShape shape;
	Vector2f velocity;
	float lifeTimer = 0.f;

	const float LIFETIME = 0.5f;
	const float START_SIZE = 4.f;

public:
	Particle();
	void SetActive(Vector2f position, Vector2f velocity);
	void Update(float deltaTime);
	bool IsActive() { return lifeTimer < LIFETIME; }
	const RectangleShape& GetShape() { return shape; }
};