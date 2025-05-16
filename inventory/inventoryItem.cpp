#include "inventoryItem.hpp"

InventoryItem::InventoryItem(const ItemType& type) {
	this->type = type;
}

InventoryItem::~InventoryItem() {

}

const ItemType& InventoryItem::getType() const {
	return type;
}