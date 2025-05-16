#pragma once

#include "enums/itemTypes.hpp"

#include "objects/SDLGameObject.hpp"
#include "inventory/inventoryItem.hpp"

class GameItem : public SDLGameObject {
private:
protected:
	ItemType type;
	bool pickedUp;
public:
	GameItem(const size_t& textureID = 0, const Float2& pos = { 0, 0 }, const Int2& srcPos = { 0, 0 }, ItemType type = ItemType::Heal);
	virtual ~GameItem();

	void setPickedUp();
	bool getPickedUp() const;

	ItemType getType() const;
};