#pragma once

#include "actors/actor.hpp"
#include "inventory/inventoryItem.hpp"

class Camera;

class Player final: public Actor {
private:
	static const size_t inventoryCapacity = 8;
	std::array<InventoryItem, inventoryCapacity> inventory;
	unsigned int exp;
	SDL_FPoint mousePos;
public:
	Player(const Float2& pos);
	virtual ~Player();

	void input(const SDL_FPoint& mousePos);
	void update(const float dt, const Camera& camera);

	bool pickupItem(const ItemType item);
	bool useItem(const size_t item);

	void setExp(const unsigned int exp);
	unsigned int getExp() const;

	const SDL_FPoint& getMousePos() const;
	float worldPosToAngle(const SDL_FPoint& worldMousePos) const;
	float posToAngle(const SDL_FPoint& mouseScreenPos, const Camera& camera) const;

	static constexpr float MOVESPEED = 80.0f;
	static constexpr unsigned int HEIGHT = 64;
	static constexpr unsigned int WIDTH = 32;
};