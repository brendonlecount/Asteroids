#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <math.h>

#include "PlayState.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "PreLevelState.h"
#include "GameOverState.h"
#include "StartMenuState.h"

using namespace std;
using namespace sf;

PlayState::PlayState(RenderWindow* window, int level, int score, int lives) :
AsteroidsState(window)
{
	this->level = level;
	this->score = score;
	this->lives = lives;
	Vector2f extents = (Vector2f)window->getSize();

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

	bucketWidth = (int)extents.x / BUCKET_COUNT;
	bucketHeight = (int)extents.y / BUCKET_COUNT;

	ship = new Ship(window, extents / 2.f, Vector2f(0.f, 0.f));
	gameObjects.push_back(ship);
	AddToBucket(ship, GetBucketIndexes(ship));

	enemy = new Enemy(window, extents / 2.f, Vector2f(0.f, 0.f));
	gameObjects.push_back(enemy);
	AddToBucket(enemy, GetBucketIndexes(enemy));

	for (int i = 0; i < GetAsteroidsSpawned(); i++) {
		GameObject* nextAsteroid = SpawnLargeAsteroid();
		gameObjects.push_back(nextAsteroid);
		AddToBucket(nextAsteroid, GetBucketIndexes(nextAsteroid));
	}
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
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		return new StartMenuState(window);
	}

	if (fireTimer <= 0.f) {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			if (spaceReleased) {
				fireTimer = FIRE_DELAY;
				Vector2f vBullet = BULLET_SPEED * Vector2f(sin(ship->GetAngle()), cos(ship->GetAngle()));
				Bullet* bullet = new Bullet(window, ship->GetPosition(), ship->GetVelocity() + vBullet, true, true);
				gameObjects.push_back(bullet);
				AddToBucket(bullet, GetBucketIndexes(bullet));
			}
		}
		else {
			spaceReleased = true;
		}
	}
	else {
		fireTimer -= deltaTime;
	}

	if (enemy->CollisionEnabled()) {
		if (enemyFireTimer <= 0) {
			enemyFireTimer = ENEMY_FIRE_DELAY;
			Vector2f vEnemyBullet = Normalize(ship->GetPosition() - enemy->GetPosition()) * BULLET_SPEED;
			GameObject* enemyBullet = new Bullet(window, enemy->GetPosition(), vEnemyBullet, false, true);
			gameObjects.push_back(enemyBullet);
			AddToBucket(enemyBullet, GetBucketIndexes(enemyBullet));
		}
		else {
			enemyFireTimer -= deltaTime;
		}
	}

	for (int i = 0; i < gameObjects.size();) {
		if (gameObjects[i]->IsDestroyed()) {
			GameObject* newObject;
			switch (gameObjects[i]->GetAsteroidsSpawned()) {
			case GameObject::AsteroidSpawn::Medium:
				for (int j = 0; j < ASTEROID_CHILD_COUNT; j++) {
					newObject = SpawnMediumAsteroid(gameObjects[i]->GetPosition(), gameObjects[i]->GetVelocity());
					gameObjects.push_back(newObject);
					AddToBucket(newObject, GetBucketIndexes(newObject));
				}
				break;
			case GameObject::AsteroidSpawn::Small:
				for (int j = 0; j < ASTEROID_CHILD_COUNT; j++) {
					newObject = SpawnSmallAsteroid(gameObjects[i]->GetPosition(), gameObjects[i]->GetVelocity());
					gameObjects.push_back(newObject);
					AddToBucket(newObject, GetBucketIndexes(newObject));
				}
				break;
			}
			if (gameObjects[i]->IsAsteroid()) {
				asteroidCount--;
				score += ASTEROID_POINTS;
				SetText(&scoreText, "Score: " + to_string(score));
			}
			RemoveFromBucket(gameObjects[i], GetBucketIndexes(gameObjects[i]));
			delete gameObjects[i];
			gameObjects.erase(gameObjects.begin() + i);
		}
		else {
			Vector2i currentBucket = GetBucketIndexes(gameObjects[i]);
			gameObjects[i]->Update(deltaTime);
			Vector2i newBucket = GetBucketIndexes(gameObjects[i]);
			if (currentBucket != newBucket) {
				RemoveFromBucket(gameObjects[i], currentBucket);
				AddToBucket(gameObjects[i], newBucket);
			}
			i++;
		}
	}

	for (int i1 = 0; i1 < BUCKET_COUNT; i1++) {
		for (int j1 = 0; j1 < BUCKET_COUNT; j1++) {
			for (int i2 = i1 - 1; i2 < i1 + 1; i2++) {
				for (int j2 = j1 - 1; j2 < j1 + 1; j2++) {
					vector<GameObject*>* bucket1 = GetBucket(Vector2i(i1, j1));
					vector<GameObject*>* bucket2 = GetBucket(Vector2i(i2, j2));
					for (int k = 0; k < bucket1->size(); k++) {
						for (int l = 0; l < bucket2->size(); l++) {
							if (IsColliding((*bucket1)[k], (*bucket2)[l])) {
								AddCollision((*bucket1)[k], (*bucket2)[l]);
							}
						}
					}
				}
			}
		}
	}

	ProcessCollisionList();

	if (asteroidCount <= 0) {
		return new PreLevelState(window, level + 1, score + LEVEL_POINTS, lives);
	}
	else if (lives < 0) {
		return new GameOverState(window, level, score);
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
	if (go1->CollisionEnabled() && go2->CollisionEnabled()) {
		Vector2f vRel = go1->GetVelocity() - go2->GetVelocity();
		Vector2f xRel = go2->GetPosition() - go1->GetPosition();
		if (Dot(vRel, xRel) > 0.f) {
			return Mag2(xRel) <= (go1->GetRadius() + go2->GetRadius()) * (go1->GetRadius() + go2->GetRadius());
		}
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

		collisions[i]->go1->OnCollide(collisions[i]->go2->DestroysAsteroids(), collisions[i]->go2->DestroysShips());
		collisions[i]->go2->OnCollide(collisions[i]->go1->DestroysAsteroids(), collisions[i]->go1->DestroysShips());
		if (collisions[i]->go1->IsShip() || collisions[i]->go2->IsShip()) {
			DeductLife();
		}
	}
	ClearCollisions();
}

Vector2f PlayState::Normalize(Vector2f v) {
	float length = sqrtf(v.x * v.x + v.y * v.y);
	if (length > 0.f) {
		return v / length;
	}
	else {
		return Vector2f(1.f, 0.f);
	}
}

void PlayState::AddCollision(GameObject* go1, GameObject* go2) {
	if (go1 == go2) {
		return;
	}

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

Vector2i PlayState::GetBucketIndexes(GameObject* go) {
	int row = (int)go->GetPosition().y / bucketHeight;
	if (row < 0) {
		row = 0;
	}
	if (row >= BUCKET_COUNT) {
		row = BUCKET_COUNT - 1;
	}
	int col = (int)go->GetPosition().x / bucketWidth;
	if (col < 0) {
		col = 0;
	}
	if (col >= BUCKET_COUNT) {
		col = BUCKET_COUNT - 1;
	}
	return Vector2i(row, col);
}

vector<GameObject*>* PlayState::GetBucket(Vector2i indexes) {
	while (indexes.x >= BUCKET_COUNT) {
		indexes.x -= BUCKET_COUNT;
	}
	while (indexes.x < 0) {
		indexes.x += BUCKET_COUNT;
	}
	while (indexes.y >= BUCKET_COUNT) {
		indexes.y -= BUCKET_COUNT;
	}
	while (indexes.y < 0) {
		indexes.y += BUCKET_COUNT;
	}
	return &buckets[indexes.x][indexes.y];
}

void PlayState::AddToBucket(GameObject* go, Vector2i indexes) {
	GetBucket(indexes)->push_back(go);
}

void PlayState::RemoveFromBucket(GameObject* go, Vector2i indexes) {
	vector<GameObject*>* bucket = GetBucket(indexes);
	vector<GameObject*>::iterator bucketIterator;
	for (bucketIterator = bucket->begin(); bucketIterator != bucket->end(); bucketIterator++) {
		if (*bucketIterator._Ptr == go) {
			bucket->erase(bucketIterator);
			return;
		}
	}
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
