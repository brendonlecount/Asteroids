#include "StartMenuState.h"
#include "PreLevelState.h"
#include "PlayStateDuel.h"

StartMenuState::StartMenuState(RenderWindow* window) :
AsteroidsState(window)
{
	Vector2f extents = (Vector2f)window->getSize();
	singlePromptText.setCharacterSize(FONT_SIZE_SMALL);
	singlePromptText.setPosition(Vector2f(extents.x / 2.f, extents.y * 0.4f));

	duelPromptText.setCharacterSize(FONT_SIZE_SMALL);
	duelPromptText.setPosition(Vector2f(extents.x / 2.f, extents.y * 0.6f));


	if (font.loadFromFile(FONT_PATH)) {
		singlePromptText.setFont(font);
		duelPromptText.setFont(font);
	}

	SetText(&singlePromptText, "Press 1 for Single Player");
	SetText(&duelPromptText, "Press 2 for Duel");
}

AsteroidsState* StartMenuState::Update(float deltaTime) {
	if (Keyboard::isKeyPressed(Keyboard::Num1)) {
		return new PreLevelState(window, 0, 0, 3);
	}
	else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
		return new PlayStateDuel(window, 5, 3);
	}
	return nullptr;
}

void StartMenuState::Draw() {
	window->clear();
	window->draw(singlePromptText);
	window->draw(duelPromptText);
}