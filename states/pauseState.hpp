#pragma once

#include "states/gameState.hpp"

class Window;
class Renderer;
class GameStateMachine;
class SoundManager;

class PauseState : public GameState {
private:
public:
	PauseState(Window* window, GameStateMachine* GSM) : GameState(window, GSM, "Pause") {}
	virtual ~PauseState();
	virtual void handleEvents();

	static void UIButtonRestart(void* ctx);
	static void UIButtonMainMenu(void* ctx);

	virtual bool onEnter(Renderer* renderer, SoundManager* soundManager);
	virtual bool onExit(SoundManager* soundManager);
};