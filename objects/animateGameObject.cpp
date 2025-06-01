#include "animateGameObject.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include <cmath>

AnimateGameObject::AnimateGameObject(const size_t& textureID, const Float2& pos, const Int2& srcPos, const bool isStatic, const bool isHover)
 : SDLGameObject(textureID, pos, srcPos, isStatic, isHover) {
	frame = 0;
	frameRate = 0.5;
	frameCols = 8;
	frameRows = 8;
	lastTime = 0;
}

AnimateGameObject::~AnimateGameObject() {

}

void AnimateGameObject::draw(Renderer* renderer, const Camera& camera, const bool& iso) const {
	//draw frame
	int x = ((frame % frameCols) * (int)size.x) + (frame % frameCols) + 1;
	int y = (int(frame / frameCols) * (int)size.y) + int(frame / frameCols) + 1;

	float hover = 0.0f;
	if (isHover) {
		hover = SDLGameObject::HOVERHEIGHT * (float)std::cos(time * SDLGameObject::HOVERSPEED);
	}

	SDL_FRect dst = { pos.x, pos.y - hover, (float)size.x, (float)size.y };
	dst = camera.worldToView(dst);
	if (iso) dst = renderer->viewToIso(dst);
	renderer->draw(textureID, { x,y, (int)size.x, (int)size.y }, dst);

}

void AnimateGameObject::update(const float dt) {
	PhysicsObject::update(dt);

	//update frame: the sum of the (col + maxCols*row)
	time += dt;

	if (time - lastTime >= frameRate) {
		lastTime = time;
		++frame;
		if (frame >= (frameCols * frameRows) - 1) { //check if greater than max
			frame = 0;
		}
	}
}

size_t& AnimateGameObject::getTextureID() {
	return textureID;
}

void AnimateGameObject::resetFrame() {
	frame = 0;
}