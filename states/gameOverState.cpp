#include "gameOverState.hpp"
#include "states/gameState.hpp"
#include "states/gameStateMachine.hpp"
#include "managers/soundManager.hpp"
#include "states/playState.hpp"
#include "states/menuState.hpp"
#include "ui/userInterface.hpp"
#include <iostream>

GameOverState::~GameOverState() {
	if (UI) delete UI;
	UI = nullptr;
}

void GameOverState::UIButtonRestart(void* ctx) {
	auto* state = static_cast<GameOverState*>(ctx);
	state->GSM->getSoundManager()->push("click");
	state->GSM->queueChange(new PlayState(state->window, state->GSM));
}

void GameOverState::UIButtonMainMenu(void* ctx) {
	auto* state = static_cast<GameOverState*>(ctx);
	state->GSM->getSoundManager()->push("click");
	state->GSM->queuePop();
	state->GSM->queueChange(new MenuState(state->window, state->GSM));
}

bool GameOverState::onEnter(Renderer* renderer, SoundManager* soundManager) {
	std::cout << "Entering game over state\n";
	UI->addElement(new UIElement({ 0, 0, vScreen::vWidth, vScreen::vHeight }, { 0, 0, 0, 55 }, true));
	UI->addElement(new UILabel({ 0, 0, 120, 25 }, Color::Green, "Game Over.", true));
	UI->addElement(new UIButton({ 0, 25, 100, 20 }, Color::White, "Restart", UIButtonRestart, this, true));
	UI->addElement(new UIButton({ 0, 50, 100, 20 }, Color::White, "Main Menu", UIButtonMainMenu, this, true));

	return true;
}

bool GameOverState::onExit(SoundManager* soundManager) {
	std::cout << "Exiting game over state\n";
	return true;
}