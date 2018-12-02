#include "StartMenuState.h"

StartMenuState::StartMenuState(RenderWindow* window) :
AsteroidsState(window)
{

}

AsteroidsState* StartMenuState::Update(float deltaTime) {
	return new PlayState(window, 0, 3, 0);
}

void StartMenuState::Draw() {

}