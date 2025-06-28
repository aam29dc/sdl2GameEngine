#pragma once

class Window;
class Renderer;
class GameStateMachine;
class UserInterface;
class SoundManager;
class UIConsole;
class Binds;

#include <string>

class GameState {
protected:
	Window* window;
	GameStateMachine* GSM;
	UserInterface* UI;
	std::string id;

	UIConsole* console;
	Binds* binds;
public:
	GameState(Window* window, GameStateMachine* GSM = nullptr, const std::string& id = "", UIConsole* console = nullptr, Binds* binds = nullptr);
	virtual ~GameState() {}
	virtual void handleEvents();

	virtual void update(SoundManager* soundManager);

	virtual void render(Renderer* renderer) const;

	virtual bool onEnter(Renderer* renderer, SoundManager* soundManager) = 0;
	virtual bool onExit(SoundManager* soundManager) = 0;

	virtual std::string getStateID();
};