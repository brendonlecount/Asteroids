#include "PlayStateDuel.h"
#include "Bullet.h"
#include "StartMenuState.h"
#include "DuelOverState.h"

PlayStateDuel::PlayStateDuel(RenderWindow* window, int level, int lives) :
	PlayState(window, level)
{
	Vector2f extents = (Vector2f)window->getSize();

	lives1Text.setPosition(extents.x * 0.25f, extents.y * 0.9f);
	lives1Text.setCharacterSize(FONT_SIZE);

	lives2Text.setPosition(extents.x * 0.75f, extents.y * 0.9f);
	lives2Text.setCharacterSize(FONT_SIZE);

	if (font.loadFromFile(FONT_PATH)) {
		lives1Text.setFont(font);
		lives2Text.setFont(font);
	}

	SetText(&lives1Text, "Lives: " + to_string(lives));
	SetText(&lives2Text, "Lives: " + to_string(lives));

	ship1 = new Ship(window, extents * 0.75f, Vector2f(0.f, 0.f), lives, false);
	gameObjects.push_back(ship1);
	AddToBucket(ship1, GetBucketIndexes(ship1));

	ship2 = new Ship(window, extents * 0.25f, Vector2f(0.f, 0.f), lives, true);
	gameObjects.push_back(ship2);
	AddToBucket(ship2, GetBucketIndexes(ship2));

	for (int i = 0; i < GetAsteroidsSpawned(); i++) {
		GameObject* nextAsteroid = SpawnLargeAsteroid();
		gameObjects.push_back(nextAsteroid);
		AddToBucket(nextAsteroid, GetBucketIndexes(nextAsteroid));
	}
}

AsteroidsState* PlayStateDuel::Update(float deltaTime) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		return new StartMenuState(window);
	}

	if (fire1Timer <= 0.f) {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			if (spaceReleased) {
				fire1Timer = FIRE_DELAY;
				Vector2f vBullet = BULLET_SPEED * Vector2f(sin(ship1->GetAngle()), cos(ship1->GetAngle()));
				Bullet* bullet = new Bullet(window, ship1->GetPosition(), ship1->GetVelocity() + vBullet, false, false);
				gameObjects.push_back(bullet);
				AddToBucket(bullet, GetBucketIndexes(bullet));
			}
		}
		else {
			spaceReleased = true;
		}
	}
	else {
		fire1Timer -= deltaTime;
	}

	if (fire2Timer <= 0.f) {
		if (Keyboard::isKeyPressed(Keyboard::RControl)) {
			fire2Timer = FIRE_DELAY;
			Vector2f vBullet = BULLET_SPEED * Vector2f(sin(ship2->GetAngle()), cos(ship2->GetAngle()));
			Bullet* bullet = new Bullet(window, ship2->GetPosition(), ship2->GetVelocity() + vBullet, false, false);
			gameObjects.push_back(bullet);
			AddToBucket(bullet, GetBucketIndexes(bullet));
		}
	}
	else {
		fire2Timer -= deltaTime;
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

	SetText(&lives1Text, "Lives: " + to_string(ship1->GetLives()));
	SetText(&lives2Text, "Lives: " + to_string(ship2->GetLives()));

	if (ship1->GetLives() < 0 || ship2->GetLives() < 0) {
		return new DuelOverState(window, ship1->GetLives(), ship2->GetLives());
	}
	else {
		return nullptr;
	}
}

void PlayStateDuel::Draw() {
	window->clear();

	window->draw(lives1Text);
	window->draw(lives2Text);

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Draw();
	}
}