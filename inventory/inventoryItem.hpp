#pragma once

#include "enums/itemTypes.hpp"

class InventoryItem {
private:
protected:
	ItemType type;
public:
	InventoryItem(const ItemType& type = ItemType::Empty);
	virtual ~InventoryItem();

	const ItemType& getType() const;
};