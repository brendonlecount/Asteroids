#include "PlayStateSingle.h"
#include "PreLevelState.h"
#include "StartMenuState.h"
#include "Bullet.h"
#include "GameOverState.h"

PlayStateSingle::PlayStateSingle(RenderWindow* window, int level, int score, int lives) :
	PlayState(window, level)
{
	this->score = score;
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

	ship = new Ship(window, extents / 2.f, Vector2f(0.f, 0.f), lives, false);
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

	if (winSoundBuffer.loadFromFile(WIN_SOUND_PATH)) {
		winSound.setBuffer(winSoundBuffer);
	}
}

AsteroidsState* PlayStateSingle::Update(float deltaTime) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		return new StartMenuState(window);
	}

	if (fireTimer <= 0.f) {
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			if (spaceReleased) {
				fireTimer = FIRE_DELAY;
				Vector2f vBullet = BULLET_SPEED * Vector2f(sin(ship->GetAngle()), cos(ship->GetAngle()));
				Bullet* bullet = new Bullet(window, ship->GetPosition(), ship->GetVelocity() + vBullet, 0.1f, true, true);
				gameObjects.push_back(bullet);
				AddToBucket(bullet, GetBucketIndexes(bullet));
				shotSound.play();
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
			GameObject* enemyBullet = new Bullet(window, enemy->GetPosition(), vEnemyBullet, 0.5f, false, true);
			gameObjects.push_back(enemyBullet);
			AddToBucket(enemyBullet, GetBucketIndexes(enemyBullet));
			shotSound.play();
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
				explosionSound.play();
				explosions.push_back(new Explosion(window, gameObjects[i]->GetPosition(), gameObjects[i]->GetVelocity(), gameObjects[i]->GetMass()));
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

	for (int i = 0; i < explosions.size();) {
		if (explosions[i]->GetActive()) {
			explosions[i]->Update(deltaTime);
			i++;
		}
		else {
			delete explosions[i];
			explosions.erase(explosions.begin() + i);
		}
	}

	SetText(&livesText, "Lives: " + to_string(ship->GetLives()));

	if (asteroidCount <= 0) {
		winSound.play();
		ship->StopThrusting();
		return new PreLevelState(window, level + 1, score + LEVEL_POINTS, ship->GetLives());
	}
	else if (ship->GetLives() < 0) {
		ship->StopThrusting();
		return new GameOverState(window, level, score);
	}
	else {
		return nullptr;
	}
}

void PlayStateSingle::Draw() {
	window->clear();

	window->draw(livesText);
	window->draw(scoreText);

	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Draw();
	}

	for (int i = 0; i < explosions.size(); i++) {
		explosions[i]->Draw();
	}
}