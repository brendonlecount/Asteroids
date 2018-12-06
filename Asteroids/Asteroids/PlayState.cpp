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

PlayState::PlayState(RenderWindow* window, int level) :
AsteroidsState(window)
{
	this->level = level;
	Vector2u extents = window->getSize();
	bucketWidth = (int)extents.x / BUCKET_COUNT;
	bucketHeight = (int)extents.y / BUCKET_COUNT;
}


PlayState::~PlayState() {
	for (int i = 0; i < gameObjects.size(); i++) {
		delete gameObjects[i];
	}
	for (int i = 0; i < collisions.size(); i++) {
		delete collisions[i];
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
