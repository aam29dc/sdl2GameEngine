#include "menuState.hpp"
#include "states/gameState.hpp"
#include "states/gameStateMachine.hpp"
#include "managers/soundManager.hpp"
#include "states/playState.hpp"
#include "ui/userInterface.hpp"
#include <iostream>

MenuState::~MenuState() {
	std::cout << "~Menu called.\n";
	if (UI) delete UI;
	UI = nullptr;
}

void MenuState::UIButtonPlay(void* ctx) {
	auto* state = static_cast<MenuState*>(ctx);
	state->GSM->getSoundManager()->push("click");
	state->GSM->queueChange(new PlayState(state->window, state->GSM));
}

bool MenuState::onEnter(Renderer* renderer, SoundManager* soundManager) {
	std::cout << "Entering menu state\n";

	UI->addElement(new UIButton({ 0,0, 100, 20 }, { 255, 255, 255, 255 }, "Play", UIButtonPlay, this, true));
	UI->addElement(new UIButton({ 0,20, 100, 20 }, { 255, 255, 255, 255 }, "Exit",
		[](void* ctx) {static_cast<GameStateMachine*>(ctx)->queuePop();}
	, this->GSM, true));

	soundManager->load("assets/sounds/click.wav", "click", soundType::SOUND_SFX);

	return true;
}

bool MenuState::onExit(SoundManager* soundManager) {
	std::cout << "Exiting menu state\n";

	return true;
}