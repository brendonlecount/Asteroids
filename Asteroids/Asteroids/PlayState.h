#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <vector>

#include "AsteroidsState.h"
#include "GameObject.h"
#include "Ship.h"
#include "Enemy.h"
#include "Explosion.h"

using namespace std;
using namespace sf;

class PlayState : public AsteroidsState {
protected:
	struct Collision {
		GameObject* go1 = nullptr;
		GameObject* go2 = nullptr;

		Collision(GameObject* go1, GameObject* go2);
		bool Equals(GameObject* go1, GameObject* go2) const;
	};

	const float FIRE_DELAY = 0.25f;
	const float BULLET_SPEED = 400.f;
	const int ASTEROID_SPAWN_MIN = 3;
	const int ASTEROID_SPAWN_MULT = 1;
	const int ASTEROID_SPEED_MAX = 100;
	const int ASTEROID_SPEED_MIN = 50;
	const float ASTEROID_SPEED_MULT = 0.25f;
	const int ASTEROID_CHILD_COUNT = 3;
	const int ASTEROID_POINTS = 500;
	const int LEVEL_POINTS = 10000;
	const float PI = 3.14159f;
	const string FONT_PATH = "Assets/Games.ttf";
	const int FONT_SIZE = 30;
	const int BUCKET_COUNT = 9;
	const float ENEMY_FIRE_DELAY = 1.25f;
	const string SHOT_SOUND_PATH = "Assets/9_mm_gunshot-mike-koenig-123.wav";
	const string EXPLOSION_SOUND_PATH = "Assets/Bomb Explosion 1-SoundBible.com-980698079.wav";
	const string ASTEROID_TEXTURE_PATH = "Assets/asteroid.jpg";

	Sound shotSound;
	SoundBuffer shotSoundBuffer;
	Sound explosionSound;
	SoundBuffer explosionSoundBuffer;
	Texture asteroidTexture;
	int asteroidCount = 0;
	int level;
	Font font;
	int bucketWidth;
	int bucketHeight;


	vector<GameObject*> buckets[9][9];
	vector<GameObject*> gameObjects;
	vector<Collision*> collisions;
	vector<Explosion*> explosions;

	bool IsColliding(GameObject* go1, GameObject* go2);
	void ProcessCollisionList();
	void AddCollision(GameObject* go1, GameObject* go2);
	void ClearCollisions() { collisions.clear(); }
	GameObject* SpawnLargeAsteroid();
	GameObject* SpawnMediumAsteroid(Vector2f position, Vector2f velocity);
	GameObject* SpawnSmallAsteroid(Vector2f position, Vector2f velocity);
	Vector2f GetRandomAsteroidVelocity();
	Vector2i GetBucketIndexes(GameObject* go);
	void AddToBucket(GameObject* go, Vector2i indexes);
	void RemoveFromBucket(GameObject* go, Vector2i indexes);
	vector<GameObject*>* GetBucket(Vector2i indexes);
	int GetAsteroidsSpawned() { return ASTEROID_SPAWN_MIN + level * ASTEROID_SPAWN_MULT; }
	float Dot(Vector2f v1, Vector2f v2);
	float Mag2(Vector2f v);
	static Vector2f Normalize(Vector2f v);

public:
	PlayState(RenderWindow* window, int level);
	~PlayState();
};