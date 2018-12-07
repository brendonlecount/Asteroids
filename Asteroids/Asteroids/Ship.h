#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "GameObject.h"
#include "Particle.h"

class Ship : public GameObject {
	Particle particles[32];
	int currentParticle = 0;
	int lives;
	float deathTimer;
	float deathBlinkTimer = 0.f;
	float particleTimer = 0.f;
	bool isPlayer2 = false;
	Vector2f particleOffset1;
	Texture particleTexture;
	Sound thrusterSound;
	SoundBuffer thrusterSoundBuffer;
	Sound deathSound;
	SoundBuffer deathSoundBuffer;
	bool thrusting = false;

	const float DEATH_TIME = 2.f;
	const float DEATH_BLINK_TIME = 0.1f;
	const float ROTATION_RATE = 180.f;
	const float DRAG_COEFF = 1.1f;
	const float THRUST = 400.f;
	const float PI = 3.14159f;
	const int PARTICLE_COUNT = 32;
	const float PARTICLE_DELAY = 0.05f;
	const float PARTICLE_SPEED = 100.f;
	const Vector2f PARTICLE_OFFSET_1 = Vector2f(-11.f, -12.f);
	const Vector2f PARTICLE_OFFSET_2 = Vector2f(11.f, -12.f);
	const string PARTICLE_TEXTURE_PATH = "Assets/whitelight.png";
	const string THRUSTER_SOUND_PATH = "Assets/Rocket Thrusters-SoundBible.com-1432176431.wav";
	const string DEATH_SOUND_PATH = "Assets/Robot_blip-Marianne_Gagnon-120342607 (1).wav";

	int GetNextParticle(int currentParticle) const;

public:
	Ship(RenderWindow* window, Vector2f position, Vector2f velocity, int lives, bool isPlayer2);
	float GetAngle() const { return -shape.getRotation() * PI / 180.f; }
	virtual void Update(float deltaTime);
	virtual void OnCollide(bool destroysAsteroids, bool destroysShips);
	virtual bool CollisionEnabled() const;
	virtual bool IsStatic() const { return false; }
	virtual bool IsDestroyed() const { return false; }
	virtual bool IsAsteroid() const { return false; }
	virtual bool IsShip() const { return true; }
	virtual AsteroidSpawn GetAsteroidsSpawned() const { return AsteroidSpawn::None; }
	virtual bool DestroysAsteroids() const { return true; }
	virtual bool DestroysShips() const { return true; }
	virtual void Draw() const;
	int GetLives() { return lives; }
	void StopThrusting() { thrusterSound.stop(); }
};