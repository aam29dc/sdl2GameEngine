#include "SDLGameObject.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "core/time.hpp"
#include <cmath>

const float SDLGameObject::HOVERHEIGHT = 4.0f;
const double SDLGameObject::HOVERSPEED = 4.0f;

SDLGameObject::SDLGameObject(const size_t& textureID, const Float2& pos, const Int2& srcPos, const bool isStatic, const bool isHover) :
	PhysicsObject(pos, {0, 0}, {SIZE, SIZE})
{
	this->textureID = textureID;
	this->srcPos = srcPos;

	collidable = true;
	this->isStatic = isStatic;
	this->isHover = isHover;
	isPlayer = false;
	drawFilled = false;
	time = 0;
}

SDLGameObject::~SDLGameObject() {

}

void SDLGameObject::draw(Renderer* renderer, const Camera& camera, const bool& iso) const {
	float hover = 0.0f;
	if (isHover) {
		hover = SDLGameObject::HOVERHEIGHT * (float)std::cos(time * SDLGameObject::HOVERSPEED);
	}

	SDL_FRect dst = { pos.x, pos.y - hover, (float)size.x, (float)size.y };
	dst = camera.worldToView(dst);
	if (iso) dst = renderer->viewToIso(dst);
	renderer->draw(textureID, { srcPos.x, srcPos.y, (int)size.x, (int)size.y }, dst);
}

void SDLGameObject::update(const float dt) {
	PhysicsObject::update(dt);
	time += dt;
}

const Int2& SDLGameObject::getSrcPos() const {
	return srcPos;
}

bool SDLGameObject::getStatic() const {
	return isStatic;
}

bool SDLGameObject::getIsPlayer() const {
	return isPlayer;
}