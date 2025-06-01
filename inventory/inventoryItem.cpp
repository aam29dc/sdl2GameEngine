#include "inventoryItem.hpp"

InventoryItem::InventoryItem(const ItemType& type) {
	this->type = type;
}

InventoryItem::~InventoryItem() {

}

const ItemType& InventoryItem::getType() const {
	return type;
}

SDL_Rect InventoryItem::getSrc(const ItemType& type) {
	SDL_Rect src = { 0 };
	src = { (1 * (int)type) + (32 * (int)type) + 1, 1, 32, 32 };
	std::cout << "type: " << (int)type << "\n";
	return src;
}