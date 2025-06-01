#pragma once

#include "states/gameState.hpp"

class Window;
class Renderer;
class GameStateMachine;
class SoundManager;

class MenuState : public GameState {
private:
public:
	MenuState(Window* window, GameStateMachine* GSM) : GameState(window, GSM, "Menu") {}
	~MenuState();

	static void UIButtonPlay(void* ctx);

	virtual bool onEnter(Renderer* renderer, SoundManager* soundManager);
	virtual bool onExit(SoundManager* soundManager);
};