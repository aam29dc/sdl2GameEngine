#pragma once

#include "math/arrayEx.hpp"
#include "objects/physicsObject.hpp"
#include <vector>

class Renderer;
class SDLGameObject;
class Camera;

class Light : public PhysicsObject {
protected:
	float z;
	float intensity;
public:
	Light(const Float2& pos, const float z, const float intensity = 1.0f);

	void drawShadows(Renderer* renderer, const std::vector<SDLGameObject*>& objects, const Camera& camera, const bool& iso = false);

	void setZ(const float z);
	float getZ() const;
};