#pragma once

#include "math/arrayEx.hpp"

class Renderer;
class SDLGameObject;
class Camera;

class MeleeAttack {
private:
protected:
	Float2 pos;
	float radius;
	float angle;
	float arc;
	SDLGameObject* owner;

	int damage;

	float startTime;

	bool canHit;
public:
	MeleeAttack(const Float2& pos, const float angle, const float arc, const float radius, const int damage, const float startTime, SDLGameObject* owner);
	virtual ~MeleeAttack();

	void draw(Renderer* renderer, const Camera& camera) const;

	const Float2& getPos() const;
	float getRadius() const;
	float getAngle() const;
	float getArc() const;
	const SDLGameObject* getOwner();

	void disableHit();
	bool getCanHit() const;
	float getStartTime() const;

	int getDamage() const;

	static constexpr float drawTime = 0.5f;
};