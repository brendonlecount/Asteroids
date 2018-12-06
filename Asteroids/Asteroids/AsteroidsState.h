#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace std;
using namespace sf;

class AsteroidsState {
protected:
	RenderWindow* window;
	Font font;

	const int FONT_SIZE_BIG = 64;
	const int FONT_SIZE_SMALL = 30;
	const string FONT_PATH = "Assets/Games.ttf";

public:
	AsteroidsState(RenderWindow* window) { this->window = window; }
	virtual AsteroidsState* Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	static void SetText(Text* text, string message);
};