#include "objects/destructible.hpp"

Destructible::Destructible(const size_t& textureID, const Float2& pos, const Int2& srcPos, const bool isStatic, const bool isHover)
	: SDLGameObject(textureID, pos, srcPos, isStatic, isHover)
{
	maxHealth = 100;
	health = 100;
}

Destructible::~Destructible() {

}