#pragma once

#include "math/arrayEx.hpp"
#include "objects/physicsObject.hpp"

class Renderer;
class Camera;

class SDLGameObject : public PhysicsObject {
private:
protected:
	size_t textureID;
	Int2 srcPos;

	bool collidable;
	bool isStatic;
	bool isHover;
	bool isPlayer;

	float time;
public:
	SDLGameObject(const size_t& textureID = 0, const Float2& pos = { 0, 0 }, const Int2& srcPos = { 0, 0 }, const bool isStatic = true, const bool isHover = false);
	virtual ~SDLGameObject();

	virtual void draw(Renderer* renderer, const Camera& camera) const override;
	virtual void update(const float dt);

	const Int2& getSrcPos() const;
	bool getStatic() const;
	bool getIsPlayer() const;

	static constexpr int SIZE = 32;
	static const float HOVERHEIGHT;
	static const double HOVERSPEED;
	static constexpr int MARGIN = 1;
};