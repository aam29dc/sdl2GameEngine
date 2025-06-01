#pragma once

class Window;
class Renderer;
class SoundManager;
class GameStateMachine;

class Game {
private:
	Window* window;
	Renderer* renderer;
	SoundManager* soundManager;
	GameStateMachine* gameStateMachine;
public:
	Game();
	~Game();

	bool init(const int windowWidth, const int windowHeight);

	void handleEvents() const;
	bool update() const;
	void render() const;

	Window* getWindow() const;

	size_t defaultTextureID;
};