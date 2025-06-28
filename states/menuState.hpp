#pragma once

#include "states/gameState.hpp"

class Window;
class Renderer;
class GameStateMachine;
class SoundManager;

class MenuState : public GameState {
private:
	size_t bgId = 0;
public:
	MenuState(Window* window, GameStateMachine* GSM, UIConsole* console, Binds* binds) : GameState(window, GSM, "Menu", console, binds) {}
	~MenuState();

	static void UIButtonPlay(void* ctx);

	void render(Renderer* renderer) const;

	virtual bool onEnter(Renderer* renderer, SoundManager* soundManager);
	virtual bool onExit(SoundManager* soundManager);
};