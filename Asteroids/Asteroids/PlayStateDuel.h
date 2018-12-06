#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "PlayState.h"
#include "Ship.h"

class PlayStateDuel : public PlayState {
	Ship* ship1;
	Ship* ship2;

	Text lives1Text;
	Text lives2Text;
	float fire1Timer;
	float fire2Timer;
	bool spaceReleased = false;

public:
	PlayStateDuel(RenderWindow* window, int level, int lives);
	virtual AsteroidsState* Update(float deltaTime);
	virtual void Draw();
};