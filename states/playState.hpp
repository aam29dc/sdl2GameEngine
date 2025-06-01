#pragma once

#include "states/gameState.hpp"

class Level;
class PauseState;
class MeleeAttack;
class Projectile;
class SDLGameObject;
class Actor;
#include "actors/player.hpp"
class Npc;
class Camera;
struct SaveData;
class UITextBox;
class Binds;

#include <vector>
#include <array>

class PlayState : public GameState {
private:
	Level* level1;

	std::vector<SDLGameObject*> allObjects;  // All level objects + actors

	std::vector<Actor*> actors;              // All actors (player + NPCs)
	std::vector<Npc*> npcs;
	Player* player;

	std::array<size_t, Player::inventoryCapacity> bagIDs = { 0 };
	bool inventoryVisible;

	std::vector<Projectile*> projectiles;
	std::vector<MeleeAttack*> swings;

	Camera* camera;
	UITextBox* combatlog;

	float timer;

	Binds* binds;
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
	void loadInventory(Renderer* renderer);

	bool showInventory();

	bool onEnter(Renderer* renderer, SoundManager* soundManager);
	bool onExit(SoundManager* soundManager);

	bool loadSave(const std::string& path);
	SaveData makeSnapShot() const;
	void clear();

	Camera* getCamera() const;
};