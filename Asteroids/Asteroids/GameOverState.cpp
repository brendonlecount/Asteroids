#include "GameOverState.h"
#include "StartMenuState.h"

GameOverState::GameOverState(RenderWindow* window, int level, int score) :
	AsteroidsState(window) 
{
	Vector2f extents = (Vector2f)window->getSize();

	gameOverText.setCharacterSize(FONT_SIZE_BIG);
	gameOverText.setPosition(extents / 2.f);

	promptText.setCharacterSize(FONT_SIZE_SMALL);
	promptText.setPosition(extents.x / 2.f, extents.y * 0.75f);

	levelText.setCharacterSize(FONT_SIZE_SMALL);
	levelText.setPosition(extents.x * 0.25f, extents.y * 0.9f);

	scoreText.setCharacterSize(FONT_SIZE_SMALL);
	scoreText.setPosition(extents.x * 0.75f, extents.y * 0.9f);


	if (font.loadFromFile(FONT_PATH)) {
		scoreText.setFont(font);
		gameOverText.setFont(font);
		promptText.setFont(font);
		levelText.setFont(font);
	}

	SetText(&gameOverText, "Game Over");
	SetText(&promptText, "Play Again? Y/N");
	SetText(&levelText, "Level: " + to_string(level + 1));
	SetText(&scoreText, "Score: " + to_string(score));
}

AsteroidsState* GameOverState::Update(float deltaTime) {
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

void GameOverState::Draw() {
	window->clear();

	window->draw(gameOverText);
	window->draw(promptText);
	window->draw(scoreText);
	window->draw(levelText);
}
