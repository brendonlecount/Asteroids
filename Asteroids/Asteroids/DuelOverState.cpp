#include "DuelOverState.h"
#include "StartMenuState.h"

DuelOverState::DuelOverState(RenderWindow* window, int player1Lives, int player2Lives) :
	AsteroidsState(window)
{
	Vector2f extents = (Vector2f)window->getSize();

	gameOverText.setCharacterSize(FONT_SIZE_BIG);
	gameOverText.setPosition(extents / 2.f);

	promptText.setCharacterSize(FONT_SIZE_SMALL);
	promptText.setPosition(extents.x / 2.f, extents.y * 0.75f);


	if (font.loadFromFile(FONT_PATH)) {
		gameOverText.setFont(font);
		promptText.setFont(font);
	}

	if (player1Lives < 0 && player2Lives < 0) {
		SetText(&gameOverText, "Draw");
	}
	else if (player1Lives < 0) {
		SetText(&gameOverText, "Player 2 Wins");
	}
	else {
		SetText(&gameOverText, "Player 1 Wins");
	}
	SetText(&promptText, "Play Again? Y/N");
}

AsteroidsState* DuelOverState::Update(float deltaTime) {
	if (Keyboard::isKeyPressed(Keyboard::Y)) {
		return new StartMenuState(window);
	}
	else if (Keyboard::isKeyPressed(Keyboard::N)) {
		window->close();
		return nullptr;
	}
	else {
		return nullptr;
	}
}

void DuelOverState::Draw() {
	window->clear();

	window->draw(gameOverText);
	window->draw(promptText);
}