#pragma once

#include "enums/itemTypes.hpp"
#include "SDL_rect.h"

class InventoryItem {
private:
protected:
	ItemType type;
public:
	InventoryItem(const ItemType& type = ItemType::Empty);
	virtual ~InventoryItem();

	const ItemType& getType() const;

	static SDL_Rect getSrc(const ItemType& type);
};