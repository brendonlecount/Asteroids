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

class GameOverState : public AsteroidsState {
	Text scoreText;
	Text gameOverText;
	Text promptText;
	Text levelText;
	Font font;

	const int FONT_SIZE_BIG = 64;
	const int FONT_SIZE_SMALL = 30;
	const string FONT_PATH = "Assets/Games.ttf";

public:
	GameOverState(RenderWindow* window, int level, int score);
	virtual AsteroidsState* Update(float deltaTime);
	virtual void Draw();
};