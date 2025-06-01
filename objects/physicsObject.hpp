#pragma once

#include "math/arrayEx.hpp"
#include "gameObject.hpp"

class Renderer;
class Camera;

class PhysicsObject : public GameObject {
private:
protected:
	Float2 pos;
	Float2 velocity;
	float speed;
	Float2 size;

	bool drawFilled;
	SDL_Color color;

	inline static size_t instanceCount = 0;
public:
	PhysicsObject(const Float2& pos = { 0, 0 }, const Float2& velocity = { 0, 0 }, const Float2& size = { 32, 32 });
	virtual ~PhysicsObject();

	static size_t getInstanceCount();

	virtual void draw(Renderer* renderer, const Camera& camera, const bool& iso = false) const override;
	virtual void update(const float dt) override;
	virtual void clean() override;

	const Float2& getSize() const;
	float getWidth() const;
	float getHeight() const;

	const Float2& getPos() const;
	float getPosX() const;	// for printing to UI
	float getPosY() const;
	const Float2& getVelocity() const;

	virtual void setPos(const Float2& pos);
	void setPosX(const float posX);
	void setPosY(const float posY);

	void setVelocity(const Float2& v);
	void setVelocityX(const float velX);
	void setVelocityY(const float velY);

	float getSpeed() const;
};