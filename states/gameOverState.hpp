#pragma once

#include "states/gameState.hpp"

class GameOverState : public GameState {
private:
public:
	GameOverState(Window* window, GameStateMachine* GSM) : GameState(window, GSM, "GameOver") {}

	static void UIButtonRestart(void* ctx);
	static void UIButtonMainMenu(void* ctx);

	virtual bool onEnter(Renderer* renderer, SoundManager* soundManager);
	virtual bool onExit(SoundManager* soundManager);
};