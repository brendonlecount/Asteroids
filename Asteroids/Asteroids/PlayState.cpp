#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "PlayState.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "StartMenuState.h"

PlayState::PlayState(RenderWindow* window, int level, int lives, int score) :
AsteroidsState(window)
{
	this->level = level;
	this->lives = lives;
	this->score = score;
	Vector2f extents = (Vector2f)window->getSize();
	ship = new Ship(window, extents / 2.f, Vector2f(0.f, 0.f));
	gameObjects.push_back(ship);
	for (int i = 0; i < GetAsteroidsSpawned(); i++) {
		gameObjects.push_back(SpawnLargeAsteroid());
	}

	livesText.setPosition(extents.x * 0.25f, extents.y * 0.9f);
	livesText.setCharacterSize(FONT_SIZE);

	scoreText.setPosition(extents.x * 0.75f, extents.y * 0.9f);
	scoreText.setCharacterSize(FONT_SIZE);

	if (font.loadFromFile(FONT_PATH)) {
		livesText.setFont(font);
		scoreText.setFont(font);
	}

	SetText(&livesText, "Lives: " + to_string(lives));
	SetText(&scoreText, "Score: " + to_string(score));
}


PlayState::~PlayState() {
	for (int i = 0; i < gameObjects.size(); i++) {
		delete gameObjects[i];
	}
	for (int i = 0; i < collisions.size(); i++) {
		delete collisions[i];
	}
}

AsteroidsState* PlayState::Update(float deltaTime) {
	if (fireTimer <= 0.f) {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			fireTimer = FIRE_DELAY;
			Vector2f vBullet = BULLET_SPEED * Vector2f(sin(ship->GetAngle()), cos(ship->GetAngle()));
			gameObjects.push_back(new Bullet(window, ship->GetPosition(), ship->GetVelocity() + vBullet));
		}
	}
	else {
		fireTimer -= deltaTime;
	}

	for (int i = 0; i < gameObjects.size();) {
		if (gameObjects[i]->IsDestroyed()) {
			switch (gameObjects[i]->GetAsteroidsSpawned()) {
			case GameObject::AsteroidSpawn::Medium:
				for (int j = 0; j < ASTEROID_CHILD_COUNT; j++) {
					gameObjects.push_back(SpawnMediumAsteroid(gameObjects[i]->GetPosition(), gameObjects[i]->GetVelocity()));
				}
				break;
			case GameObject::AsteroidSpawn::Small:
				for (int j = 0; j < ASTEROID_CHILD_COUNT; j++) {
					gameObjects.push_back(SpawnSmallAsteroid(gameObjects[i]->GetPosition(), gameObjects[i]->GetVelocity()));
				}
				break;
			}
			if (gameObjects[i]->IsAsteroid()) {
				asteroidCount--;
				score += ASTEROID_POINTS;
				SetText(&scoreText, "Score: " + to_string(score));
			}
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
		}
		else {
			gameObjects[i]->Update(deltaTime);
			i++;
		}
	}
	for (int i = 0; i < gameObjects.size(); i++) {
		for (int j = i; j < gameObjects.size(); j++) {
			if (IsColliding(gameObjects[i], gameObjects[j])) {
				AddCollision(gameObjects[i], gameObjects[j]);
			}
		}
	}
	ProcessCollisionList();

	if (asteroidCount <= 0) {
		return new PlayState(window, level + 1, lives, score + LEVEL_POINTS);
	}
	else if (lives <= 0) {
		return new StartMenuState(window);
	}
	else {
		return nullptr;
	}
}

void PlayState::Draw() {
	window->clear();

	window->draw(livesText);
	window->draw(scoreText);

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Draw();
	}
}

bool PlayState::IsColliding(GameObject* go1, GameObject* go2) {
	Vector2f vRel = go1->GetVelocity() - go2->GetVelocity();
	Vector2f xRel = go2->GetPosition() - go1->GetPosition();
	if (Dot(vRel, xRel) > 0.f) {
		return Mag2(xRel) <= (go1->GetRadius() + go2->GetRadius()) * (go1->GetRadius() + go2->GetRadius());
	}
	return false;
}

void PlayState::ProcessCollisionList() {
	for (int i = 0; i < collisions.size(); i++) {
		// it's been a long time since I took dynamics, so...
		// https://en.wikipedia.org/wiki/Elastic_collision
		Vector2f v1 = collisions[i]->go1->GetVelocity();
		Vector2f v2 = collisions[i]->go2->GetVelocity();
		Vector2f x1 = collisions[i]->go1->GetPosition();
		Vector2f x2 = collisions[i]->go2->GetPosition();
		float m1 = collisions[i]->go1->GetMass();
		float m2 = collisions[i]->go2->GetMass();

		Vector2f v1Final = v1 - (2 * m2) * Dot(v1 - v2, x1 - x2) * (x1 - x2) / (Mag2(x1 - x2) * (m1 + m2));
		Vector2f v2Final = v2 - (2 * m1) * Dot(v2 - v1, x2 - x1) * (x2 - x1) / (Mag2(x2 - x1) * (m1 + m2));

		collisions[i]->go1->SetVelocity(v1Final);
		collisions[i]->go2->SetVelocity(v2Final);

		collisions[i]->go1->OnCollide(collisions[i]->go2->DestroysAsteroids());
		collisions[i]->go2->OnCollide(collisions[i]->go1->DestroysAsteroids());
		if (collisions[i]->go1->IsShip() || collisions[i]->go2->IsShip()) {
			DeductLife();
		}
	}
	ClearCollisions();
}

void PlayState::AddCollision(GameObject* go1, GameObject* go2) {
	for (int i = 0; i < collisions.size(); i++) {
		if (collisions[i]->Equals(go1, go2)) {
			return;
		}
	}
	collisions.push_back(new Collision(go1, go2));
}

float PlayState::Dot(Vector2f v1, Vector2f v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float PlayState::Mag2(Vector2f v) {
	return v.x * v.x + v.y * v.y;
}

void PlayState::DeductLife() {
	lives--;
	SetText(&livesText, "Lives: " + to_string(lives));
}


GameObject* PlayState::SpawnLargeAsteroid() {
	asteroidCount++;
	Vector2f position = Vector2f((float)(rand() % window->getSize().x), (float)(rand() % window->getSize().y));
	Vector2f velocity = GetRandomAsteroidVelocity();
	return new Asteroid(window, 40.f, 2.f, position, velocity, GameObject::AsteroidSpawn::Medium);
}

GameObject* PlayState::SpawnMediumAsteroid(Vector2f position, Vector2f velocity) {
	asteroidCount++;
	velocity += GetRandomAsteroidVelocity();
	return new Asteroid(window, 20.f, 1.f, position, velocity, GameObject::AsteroidSpawn::Small);
}

GameObject* PlayState::SpawnSmallAsteroid(Vector2f position, Vector2f velocity) {
	asteroidCount++;
	velocity += GetRandomAsteroidVelocity();
	return new Asteroid(window, 10.f, 0.5f, position, velocity, GameObject::AsteroidSpawn::None);
}

Vector2f PlayState::GetRandomAsteroidVelocity() {
	float angle = (float)(rand() % 360) * PI / 180.f;
	float speed = (float)(ASTEROID_SPEED_MIN + rand() % (ASTEROID_SPEED_MAX - ASTEROID_SPEED_MIN));
	speed *= (1 + level * ASTEROID_SPEED_MULT);
	return Vector2f(speed * cos(angle), speed * sin(angle));
}

PlayState::Collision::Collision(GameObject* go1, GameObject* go2) {
	this->go1 = go1;
	this->go2 = go2;
}

bool PlayState::Collision::Equals(GameObject* go1, GameObject* go2) const {
	if (this->go1 == go1) {
		return this->go2 == go2;
	}
	else if (this->go1 == go2) {
		return this->go2 == go1;
	}
	return false;
}
