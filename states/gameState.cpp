#include "states/gameState.hpp"
#include "ui/userInterface.hpp"

GameState::GameState(Window* window, GameStateMachine* GSM, const std::string& id) : window(window), GSM(GSM), id(id) {
	UI = new UserInterface(window);
}

void GameState::handleEvents() {
	UI->handleEvents();
}

void GameState::update(SoundManager* soundManager) {
	UI->update();
}

void GameState::render(Renderer* renderer) const {
	UI->render(renderer);
}

std::string GameState::getStateID() { 
	return id;
}