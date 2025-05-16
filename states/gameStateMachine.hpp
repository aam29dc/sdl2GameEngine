#pragma once

class Renderer;
class Window;
class SoundManager;
class GameState;

#include <vector>
#include <queue>

class GameStateMachine {
public:
	enum class TransitionType { Push, Pop, Change };
private:
	Window* window;
	Renderer* renderer;
	SoundManager* soundManager;

	std::vector<GameState*> gameStates;
	struct PendingTransition { TransitionType type;	GameState* state; };
	std::queue<PendingTransition> transitions;
public:
	GameStateMachine(Window* window, Renderer* renderer, SoundManager* soundManager) : window(window), renderer(renderer), soundManager(soundManager) {};
	~GameStateMachine();

	void handleEvents();
	bool update();
	void render(Renderer* renderer) const;

	void queuePush(GameState* state);
	void queueChange(GameState* state);
	void queuePop();

	void processTransitions();

	GameState* getCurrentState() const;
	SoundManager* getSoundManager() const;
};