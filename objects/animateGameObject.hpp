#pragma once

#include "objects/SDLGameObject.hpp"

struct frameData {

};

class Renderer;
class Camera;

class AnimateGameObject : public SDLGameObject {
private:
protected:
	double frameRate;
	double lastTime;

	unsigned int frame;

	unsigned int frameCols;
	unsigned int frameRows;
public:
	AnimateGameObject(const size_t& textureID = 0, const Float2& pos = { 0, 0 }, const Int2& srcPos = { 0,0 }, const bool isStatic = true, const bool isHover = false);
	virtual ~AnimateGameObject();

	virtual void draw(Renderer* renderer, const Camera& camera) const override;
	virtual void update(const float dt) override;

	void resetFrame();

	size_t& getTextureID();
};