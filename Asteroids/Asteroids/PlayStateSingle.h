#pragma once

#include "PlayState.h"
#include "Ship.h"
#include "Enemy.h"

class PlayStateSingle : public PlayState {
	Ship* ship;
	Enemy* enemy;
	int score;
	float fireTimer = 0.f;
	float enemyFireTimer = 0.f;
	Text livesText;
	Text scoreText;
	bool spaceReleased = false;

public:
	PlayStateSingle(RenderWindow* window, int level, int score, int lives);
	virtual AsteroidsState* Update(float deltaTime);
	virtual void Draw();
};