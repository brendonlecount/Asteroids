#include "AsteroidsState.h"


void AsteroidsState::SetText(Text* text, string message) {
	text->setString(message);
	FloatRect textRect = text->getLocalBounds();
	text->setOrigin(textRect.left + textRect.width / 2.f, textRect.top + textRect.height / 2.f);
}