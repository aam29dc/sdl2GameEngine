#pragma once

#include "actors/actor.hpp"
#include "inventory/inventoryItem.hpp"

class Camera;
class Binds;

class Player final: public Actor {
public:
	static const size_t inventoryCapacity = 8;
private:
	std::array<InventoryItem, inventoryCapacity> inventory;
	unsigned int exp;
	SDL_FPoint mousePos;

	float speedTime;
	bool speedActive;

	Binds* binds;
	UITextBox* console;
public:
	Player(const Float2& pos = {0}, UITextBox* const combatlog = nullptr, Binds* const binds = nullptr, UITextBox* const console = nullptr);
	virtual ~Player();

	void input(const SDL_FPoint& mousePos);
	void update(const float dt, const Camera& camera);

	int pickupItem(const ItemType item);
	bool useItem(const size_t item);

	void setExp(const unsigned int exp);
	unsigned int getExp() const;

	const SDL_FPoint& getMousePos() const;
	float worldPosToAngle(const SDL_FPoint& worldMousePos) const;
	float posToAngle(const SDL_FPoint& mouseScreenPos, const Camera& camera) const;

	void takeDamage(const int damage) override;

	static constexpr float MOVESPEED = 80.0f;
	static constexpr unsigned int HEIGHT = 64;
	static constexpr unsigned int WIDTH = 32;
};