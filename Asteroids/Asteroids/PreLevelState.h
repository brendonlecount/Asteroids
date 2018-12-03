#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "AsteroidsState.h"

using namespace std;
using namespace sf;

class PreLevelState : public AsteroidsState {
	Text scoreText;
	Text livesText;
	Text promptText;
	Text levelText;
	Font font;

	int level;
	int score;
	int lives;
	bool spaceReleased = false;

	const int FONT_SIZE_BIG = 64;
	const int FONT_SIZE_SMALL = 30;
	const string FONT_PATH = "Assets/Games.ttf";

public:
	PreLevelState(RenderWindow* window, int level, int score, int lives);
	virtual AsteroidsState* Update(float deltaTime);
	virtual void Draw();
};