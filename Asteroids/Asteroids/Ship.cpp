#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "Ship.h"

using namespace std;
using namespace sf;

Ship::Ship(RenderWindow* window, Vector2f position, Vector2f velocity, int lives, bool isPlayer2) :
GameObject(window, 20.f, 1.f, position, velocity)
{
	this->lives = lives;
	this->isPlayer2 = isPlayer2;
	deathTimer = DEATH_TIME;
	shape.setRotation(180.f);
	if (texture.loadFromFile("Assets/ship.jpg")) {
		shape.setTexture(&texture);
	}
	if (isPlayer2) {
		shape.setFillColor(Color::Green);
	}
}

void Ship::Update(float deltaTime) {
	if (deathTimer > 0.f) {
		deathTimer -= deltaTime;
		if (deathBlinkTimer > 2.f * DEATH_BLINK_TIME) {
			deathBlinkTimer = 0.f;
		}
		else {
			deathBlinkTimer += deltaTime;
		}
	}
	else {
		deathBlinkTimer = 0.f;
	}

	if ((!isPlayer2 && Keyboard::isKeyPressed(Keyboard::A)) || (isPlayer2 && Keyboard::isKeyPressed(Keyboard::Left))) {
		shape.rotate(-ROTATION_RATE * deltaTime);
	}
	if ((!isPlayer2 && Keyboard::isKeyPressed(Keyboard::D)) || (isPlayer2 && Keyboard::isKeyPressed(Keyboard::Right))) {
		shape.rotate(ROTATION_RATE * deltaTime);
	}
	if ((!isPlayer2 && Keyboard::isKeyPressed(Keyboard::W)) || (isPlayer2 && Keyboard::isKeyPressed(Keyboard::Up))) {
		Vector2f thrust = THRUST * Vector2f(sin(GetAngle()), cos(GetAngle()));
		velocity += deltaTime * thrust / mass;
	}
	if ((!isPlayer2 && Keyboard::isKeyPressed(Keyboard::S)) || (isPlayer2 && Keyboard::isKeyPressed(Keyboard::Down))) {
		Vector2f thrust = THRUST * Vector2f(sin(GetAngle()), cos(GetAngle()));
		velocity -= deltaTime * thrust / mass;
	}

	velocity -= velocity * DRAG_COEFF * deltaTime;

	ApplyVelocity(deltaTime);
}

void Ship::OnCollide(bool destroysAsteroids, bool destroysShips) {
	if (destroysShips) {
		deathTimer = DEATH_TIME;
		deathBlinkTimer = 0.f;
		lives--;
	}
}

bool Ship::CollisionEnabled() const {
	if (deathTimer > 0.f) {
		return false;
	}
	return true;
}

void Ship::Draw() const {
	if (deathBlinkTimer < DEATH_BLINK_TIME) {
		window->draw(shape);
	}
}