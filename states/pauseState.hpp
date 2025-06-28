#pragma once

#include "states/gameState.hpp"

class Window;
class Renderer;
class GameStateMachine;
class SoundManager;

class PauseState : public GameState {
private:
public:
	PauseState(Window* window, GameStateMachine* GSM, UIConsole* console, Binds* binds) : GameState(window, GSM, "Pause", console, binds) {}
	virtual ~PauseState();
	virtual void handleEvents();

	static void UIButtonRestart(void* ctx);
	static void UIButtonMainMenu(void* ctx);

	void render(Renderer* renderer) const;

	virtual bool onEnter(Renderer* renderer, SoundManager* soundManager);
	virtual bool onExit(SoundManager* soundManager);
};