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

public:
	AsteroidsState(RenderWindow* window) { this->window = window; }
	virtual AsteroidsState* Update(float deltaTime) = 0;
	virtual void Draw() = 0;
	static void SetText(Text* text, string message);
};