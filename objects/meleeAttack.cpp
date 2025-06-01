#include "meleeAttack.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "objects/SDLGameObject.hpp"
#include "gfx/SDL2_gfxPrimitives.h"

MeleeAttack::MeleeAttack(const Float2& pos, const float angle, const float arc, const float radius, const int damage, const float startTime, SDLGameObject* owner) {
	this->pos = pos;
	this->angle = angle;
	this->arc = arc;
	this->radius = radius;
	this->owner = owner;

	this->damage = damage;

	canHit = true;
	this->startTime = startTime;
}

MeleeAttack::~MeleeAttack() {

}

void MeleeAttack::draw(Renderer* renderer, const Camera& camera, const bool& iso) const {
	Float2 newPos = pos;
	newPos = camera.worldToView(newPos);
	if (iso) newPos = renderer->viewToIso(newPos);
	newPos = renderer->scale(newPos);
	circleRGBA(renderer->sdlRenderer, (Sint16)newPos.x, (Sint16)newPos.y, (Sint16)radius, 255, 255, 0, 255);
}

const Float2& MeleeAttack::getPos() const {
	return pos;
}

float MeleeAttack::getRadius() const {
	return radius;
}

float MeleeAttack::getAngle() const {
	return angle;
}
float MeleeAttack::getArc() const {
	return arc;
}

const SDLGameObject* MeleeAttack::getOwner() {
	return owner;
}

void MeleeAttack::disableHit() {
	canHit = false;
}

bool MeleeAttack::getCanHit() const {
	return canHit;
}

float MeleeAttack::getStartTime() const {
	return startTime;
}

int MeleeAttack::getDamage() const {
	return damage;
}