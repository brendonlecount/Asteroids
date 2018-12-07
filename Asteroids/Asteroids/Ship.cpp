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
	if (!isPlayer2 && texture.loadFromFile("Assets/ship.jpg")) {
		shape.setTexture(&texture);
	}
	if (isPlayer2 && texture.loadFromFile("Assets/ship2.jpg")) {
		shape.setTexture(&texture);
	}
	particleTexture.loadFromFile(PARTICLE_TEXTURE_PATH);
	if (thrusterSoundBuffer.loadFromFile(THRUSTER_SOUND_PATH)) {
		thrusterSound.setBuffer(thrusterSoundBuffer);
	}
	if (deathSoundBuffer.loadFromFile(DEATH_SOUND_PATH)) {
		deathSound.setBuffer(deathSoundBuffer);
	}
	thrusterSound.setLoop(true);
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
		Vector2f thrustDirection = Vector2f(sin(GetAngle()), cos(GetAngle()));
		velocity += deltaTime * THRUST * thrustDirection / mass;
		if (!thrusting) {
			thrusting = true;
			thrusterSound.play();
		}
		if (particleTimer <= 0.f) {
			particleTimer = PARTICLE_DELAY;
			float cosA = cos(-GetAngle());
			float sinA = sin(-GetAngle());
			Vector2f pos;
			pos.x = PARTICLE_OFFSET_1.x * cosA - PARTICLE_OFFSET_1.y * sinA;
			pos.y = PARTICLE_OFFSET_1.x * sinA + PARTICLE_OFFSET_1.y * cosA;
			particles[currentParticle].SetActive(GetPosition() + pos, velocity - thrustDirection * PARTICLE_SPEED, 4.f, 0.5f, particleTexture, Color(50, 70, 255, 255));
			currentParticle = GetNextParticle(currentParticle);
			pos.x = PARTICLE_OFFSET_2.x * cosA - PARTICLE_OFFSET_2.y * sinA;
			pos.y = PARTICLE_OFFSET_2.x * sinA + PARTICLE_OFFSET_2.y * cosA;
			particles[currentParticle].SetActive(GetPosition() + pos, velocity - thrustDirection * PARTICLE_SPEED, 4.f, 0.5f, particleTexture, Color(50, 70, 255, 255));
			currentParticle = GetNextParticle(currentParticle);
		}
		else {
			particleTimer -= deltaTime;
		}
	}
	else {
		if (thrusting) {
			thrusterSound.stop();
			thrusting = false;
		}
		particleTimer = 0.f;
	}
	if ((!isPlayer2 && Keyboard::isKeyPressed(Keyboard::S)) || (isPlayer2 && Keyboard::isKeyPressed(Keyboard::Down))) {
		Vector2f thrust = THRUST * Vector2f(sin(GetAngle()), cos(GetAngle()));
		velocity -= deltaTime * thrust / mass;
	}

	velocity -= velocity * DRAG_COEFF * deltaTime;

	ApplyVelocity(deltaTime);

	for (int i = 0; i < PARTICLE_COUNT; i++) {
		particles[i].Update(deltaTime);
	}
}

void Ship::OnCollide(bool destroysAsteroids, bool destroysShips) {
	if (destroysShips) {
		deathTimer = DEATH_TIME;
		deathBlinkTimer = 0.f;
		lives--;
		deathSound.play();
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
	for (int i = 0, j = GetNextParticle(currentParticle); i < PARTICLE_COUNT; i++, j = GetNextParticle(j)) {
		if (particles[j].IsActive()) {
			window->draw(particles[j].GetShape());
		}
	}
}

int Ship::GetNextParticle(int currentParticle) const {
	currentParticle++;
	if (currentParticle < PARTICLE_COUNT) {
		return currentParticle;
	}
	else {
		return 0;
	}
}