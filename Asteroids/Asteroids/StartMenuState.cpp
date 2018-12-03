#include "StartMenuState.h"
#include "PreLevelState.h"

StartMenuState::StartMenuState(RenderWindow* window) :
AsteroidsState(window)
{

}

AsteroidsState* StartMenuState::Update(float deltaTime) {
	return new PreLevelState(window, 0, 0, 3);
}

void StartMenuState::Draw() {

}