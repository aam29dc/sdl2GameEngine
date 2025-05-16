#pragma once

#include "objects/SDLGameObject.hpp"

enum class BounceAxis { NONE, X, Y, XY };

class Projectile final: public PhysicsObject {
private:
protected:
	SDLGameObject* owner;
	bool collided;
	unsigned int bounces;
	unsigned int prevBounces;
	unsigned int maxBounces;
	BounceAxis lastBounceAxis;

	int damage;
public:
	Projectile(const Float2& pos, const Float2& velocity, const unsigned int damage, const unsigned int maxBounces, const SDL_Color& color, SDLGameObject* owner = nullptr);
	virtual ~Projectile();

	virtual void update(const float dt);

	void setCollided();
	void setBounceAxis(const BounceAxis& lastBounceAxis);

	bool getCollided() const;
	unsigned int getBounces() const;
	unsigned int getMaxBounces() const;
	const SDLGameObject* getOwner();
	int getDamage() const;
};