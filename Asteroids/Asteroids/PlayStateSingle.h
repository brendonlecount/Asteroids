#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "PlayState.h"
#include "Ship.h"
#include "Enemy.h"

class PlayStateSingle : public PlayState {
	Sound winSound;
	SoundBuffer winSoundBuffer;
	Ship* ship;
	Enemy* enemy;
	int score;
	float fireTimer = 0.f;
	float enemyFireTimer = 0.f;
	Text livesText;
	Text scoreText;
	bool spaceReleased = false;

	const string WIN_SOUND_PATH = "Assets/5_Sec_Crowd_Cheer-Mike_Koenig-1562033255.wav";

public:
	PlayStateSingle(RenderWindow* window, int level, int score, int lives);
	virtual AsteroidsState* Update(float deltaTime);
	virtual void Draw();
};