#include "states/gameState.hpp"
#include "ui/userInterface.hpp"

GameState::GameState(Window* window, GameStateMachine* GSM, const std::string& id, UIConsole* console, Binds* binds) : window(window), GSM(GSM), id(id) {
	UI = new UserInterface(window, id);
	this->binds = binds;
	this->console = console;
}

void GameState::handleEvents() {
	UI->handleEvents();

	if (console->getVisible()) {
		return;
	}
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