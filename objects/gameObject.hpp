#pragma once

class Renderer;
class Camera;

class GameObject {
private:
protected:
	GameObject();
	virtual ~GameObject();
public:
	virtual void draw(Renderer* renderer, const Camera& camera) const = 0;
	virtual void update(const float dt) = 0;
	virtual void clean() = 0;
};