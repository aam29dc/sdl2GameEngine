#include "gameItem.hpp"

//SDLGameObject(const size_t& textureID, const Float2& pos, const Int2& srcPos, const bool& isStatic, const bool& isHover)
GameItem::GameItem(const size_t& textureID, const Float2& pos, const Int2& srcPos, ItemType type) : 
	SDLGameObject(textureID, pos, srcPos, true, true)
{
	this->srcPos = srcPos;

	this->type = type;
	pickedUp = false;
	collidable = false;
}

GameItem::~GameItem() {

}

void GameItem::setPickedUp() {
	pickedUp = true;
}

bool GameItem::getPickedUp() const {
	return pickedUp;
}

ItemType GameItem::getType() const {
	return type;
}