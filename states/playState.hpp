#pragma once

#include "states/gameState.hpp"

class Level;
class PauseState;
class MeleeAttack;
class Projectile;
class SDLGameObject;
class Actor;
class Player;
class Npc;
class Camera;
struct SaveData;

#include <vector>

class PlayState : public GameState {
private:
	Level* level1;

	std::vector<SDLGameObject*> allObjects;  // All level objects + actors

	std::vector<Actor*> actors;              // All actors (player + NPCs)
	std::vector<Npc*> npcs;
	Player* player;

	std::vector<Projectile*> projectiles;
	std::vector<MeleeAttack*> swings;

	Camera* camera;

	float timer;
public:
	PlayState(Window* window, GameStateMachine* GSM);
	~PlayState();

	float getTimer() const { return timer; }

	void handleEvents();
	void update(SoundManager* soundManager);
	void render(Renderer* renderer) const;

	inline void refreshActors();
	inline void refreshAllObjects();

	void loadTextures(Renderer* renderer);

	bool onEnter(Renderer* renderer, SoundManager* soundManager);
	bool onExit(SoundManager* soundManager);

	bool loadSave(const std::string& path);
	SaveData makeSnapShot() const;
	void clear();

	Camera* getCamera() const;
};