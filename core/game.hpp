#pragma once

class Window;
class Renderer;
class SoundManager;
class GameStateMachine;
class Player;
class Binds;
class UIConsole;

class Game {
private:
	Window* window;
	Renderer* renderer;
	SoundManager* soundManager;
	GameStateMachine* gameStateMachine;

	Binds* binds;
	UIConsole* console;
public:
	Game();
	~Game();

	bool init(const int windowWidth, const int windowHeight);

	void handleEvents() const;
	bool update() const;
	void render() const;

	void registerCvars();

	Window* getWindow() const;

	Player* getCurrentPlayer();

	size_t defaultTextureID;
};