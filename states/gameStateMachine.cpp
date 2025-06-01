#include "gameStateMachine.hpp"
#include "states/gameState.hpp"
#include "managers/soundManager.hpp"

GameStateMachine::~GameStateMachine() {
	while (!gameStates.empty()) {
		gameStates.back()->onExit(soundManager);
		delete gameStates.back();
		gameStates.pop_back();
	}
}

void GameStateMachine::queuePush(GameState* state) {
	transitions.push({ TransitionType::Push, state });
}

void GameStateMachine::queueChange(GameState* state) {
	transitions.push({ TransitionType::Change, state });
}

void GameStateMachine::queuePop() {
	transitions.push({ TransitionType::Pop, nullptr });
}
#include <iostream>
void GameStateMachine::processTransitions() {
	while (!transitions.empty()) {
		auto& t = transitions.front();

		switch (t.type) {
		case TransitionType::Push:
			if (t.state) {
				t.state->onEnter(renderer, soundManager);
				gameStates.push_back(t.state);
			}
			break;

		case TransitionType::Change:
			if (!gameStates.empty()) {
				if (gameStates.back()->onExit(soundManager)) {
					delete gameStates.back();
					gameStates.pop_back();
					std::cout << "pop1\n";
				}
			}
			if (t.state) {
				t.state->onEnter(renderer, soundManager);
				gameStates.push_back(t.state);
			}
			break;

		case TransitionType::Pop:
			if (!gameStates.empty()) {
				if (gameStates.back()->onExit(soundManager)) {
					delete gameStates.back();
					gameStates.pop_back();
				}
			}
			break;
		}
		transitions.pop();
	}
}

void GameStateMachine::handleEvents() {
	if (!gameStates.empty()) {
		gameStates.back()->handleEvents();
	}
}

bool GameStateMachine::update() {
	processTransitions();

	if (!gameStates.empty()) {
		gameStates.back()->update(soundManager);
		return false;
	}
	return true;
}

void GameStateMachine::render(Renderer* renderer) const {
	if (!gameStates.empty()) {
		for (int i = 0; i < gameStates.size();i++) {
			if (gameStates[i]->getStateID() == "Play") {
				gameStates[i]->render(renderer);
			}
		}
		gameStates.back()->render(renderer);
	}
}

GameState* GameStateMachine::getCurrentState() const {
	if (!gameStates.empty()) {
		return gameStates.back();
	}
	else return nullptr;
}

SoundManager* GameStateMachine::getSoundManager() const {
	return soundManager;
}