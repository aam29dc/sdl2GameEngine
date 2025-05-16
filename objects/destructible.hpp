#pragma once

#include "objects/SDLGameObject.hpp"
#include "components/health.hpp"

class Destructible : public SDLGameObject, public Health {
protected:
public:
	Destructible(const size_t& textureID = 0, const Float2& pos = { 0, 0 }, const Int2& srcPos = { 0,0 }, const bool isStatic = true, const bool isHover = false);
	virtual ~Destructible();
};