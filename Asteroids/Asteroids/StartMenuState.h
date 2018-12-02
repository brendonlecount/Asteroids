#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include "AsteroidsState.h"
#include "PlayState.h"

using namespace std;
using namespace sf;

class StartMenuState : public AsteroidsState {
public:
	StartMenuState(RenderWindow* window);
	virtual AsteroidsState* Update(float deltaTime);
	virtual void Draw();

};
