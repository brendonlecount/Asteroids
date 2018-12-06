#include "PreLevelState.h"
#include "AsteroidsState.h"
#include "PlayStateSingle.h"

PreLevelState::PreLevelState(RenderWindow* window, int level, int score, int lives) :
	AsteroidsState(window)
{
	this->level = level;
	this->score = score;
	this->lives = lives;
	Vector2f extents = (Vector2f)window->getSize();

	levelText.setCharacterSize(FONT_SIZE_BIG);
	levelText.setPosition(extents / 2.f);

	promptText.setCharacterSize(FONT_SIZE_SMALL);
	promptText.setPosition(extents.x / 2.f, extents.y * 0.75f);

	livesText.setCharacterSize(FONT_SIZE_SMALL);
	livesText.setPosition(extents.x * 0.25f, extents.y * 0.9f);

	scoreText.setCharacterSize(FONT_SIZE_SMALL);
	scoreText.setPosition(extents.x * 0.75f, extents.y * 0.9f);


	if (font.loadFromFile(FONT_PATH)) {
		scoreText.setFont(font);
		livesText.setFont(font);
		promptText.setFont(font);
		levelText.setFont(font);
	}

	SetText(&levelText, "Level " + to_string(level + 1));
	SetText(&promptText, "Press space to start");
	SetText(&livesText, "Lives: " + to_string(lives));
	SetText(&scoreText, "Score: " + to_string(score));
}

AsteroidsState* PreLevelState::Update(float deltaTime) {
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		if (spaceReleased) {
			return new PlayStateSingle(window, level, score, lives);
		}	
	}
	else {
		spaceReleased = true;
	}
	return nullptr;
}

void PreLevelState::Draw() {
	window->clear();
	window->draw(levelText);
	window->draw(promptText);
	window->draw(livesText);
	window->draw(scoreText);
}