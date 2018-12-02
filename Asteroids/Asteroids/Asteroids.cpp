#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

using namespace std;
using namespace sf;

#include "AsteroidsState.h"
#include "StartMenuState.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;


int main()
{
	srand((unsigned)time(nullptr));
	RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Asteroids");

	AsteroidsState* gameState = new StartMenuState(&window);

	Clock clock;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				return 0;
			}
		}

		AsteroidsState* nextState = gameState->Update(clock.restart().asSeconds());

		if (nextState != nullptr) {
			delete gameState;
			gameState = nextState;
		}

		gameState->Draw();
		window.display();
	}

	return 0;
}