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
	float startSize;
	float lifeTime = 1.f;

public:
	Particle();
	void SetActive(Vector2f position, Vector2f velocity, float size, float lifeTime, Texture& texture, Color color);
	void Update(float deltaTime);
	bool IsActive() const { return lifeTimer < lifeTime; }
	const RectangleShape& GetShape() const { return shape; }
};