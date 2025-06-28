#include "pauseState.hpp"
#include "states/gameState.hpp"
#include "states/gameStateMachine.hpp"
#include "managers/soundManager.hpp"
#include "core/input.hpp"
#include "enums/virtualScreen.hpp"
#include "states/menuState.hpp"
#include "states/playState.hpp"
#include "ui/userInterface.hpp"
#include "core/binds.hpp"
#include <iostream>

void PauseState::handleEvents() {
	UI->handleEvents();
	if (Input::getInputHandler()->isKeyReleased(SDL_SCANCODE_ESCAPE)) {
		GSM->queuePop();
	}
}

PauseState::~PauseState() {
	if (UI) delete UI;
	UI = nullptr;
}

void PauseState::UIButtonRestart(void* ctx) {
	auto* state = static_cast<PauseState*>(ctx);
	state->GSM->getSoundManager()->push("click");
	state->GSM->queuePop();
	state->GSM->queueChange(new PlayState(state->window, state->GSM, state->console, state->binds));
}

void PauseState::UIButtonMainMenu(void* ctx) {
	auto* state = static_cast<PauseState*>(ctx);
	state->GSM->getSoundManager()->push("click");
	state->GSM->queuePop();
	state->GSM->queueChange(new MenuState(state->window, state->GSM, state->console, state->binds));
}

bool PauseState::onEnter(Renderer* renderer, SoundManager* soundManager) {
	std::cout << "Entering pause state\n";
	UI->addElement(new UIElement({ 0, 0, vScreen::vWidth, vScreen::vHeight }, { 0, 0, 0, 55 }, true));
	UI->addElement(new UILabel({ 0, 0, 120, 25 }, Color::Green, "Game Paused.", true));
	UI->addElement(new UIButton({ 0,25, 100, 20 }, Color::White, "Restart", UIButtonRestart, this, true));
	UI->addElement(new UIButton({ 0,50, 100, 20 }, Color::White, "Main Menu", UIButtonMainMenu, this, true));

	return true;
}

void PauseState::render(Renderer* renderer) const {
	UI->render(renderer);
}

bool PauseState::onExit(SoundManager* soundManager) {
	std::cout << "Exiting pause state\n";
	return true;
}