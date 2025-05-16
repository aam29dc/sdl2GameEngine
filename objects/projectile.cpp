#include "projectile.hpp"

Projectile::Projectile(const Float2& pos, const Float2& velocity, const unsigned int damage, const unsigned int maxBounces, const SDL_Color& color, SDLGameObject* owner)
: PhysicsObject(pos, velocity, {4.0f, 4.0f}) {
	this->color = color;

	this->damage = damage;
	this->owner = owner;

	collided = false;
	bounces = 0;
	prevBounces = 0;
	this->maxBounces = maxBounces;
	lastBounceAxis = BounceAxis::NONE;
}

Projectile::~Projectile() {

}

void Projectile::setCollided() {
	collided = true;
	++bounces;
}

void Projectile::update(const float dt) {
	if (bounces > prevBounces) {
		switch (lastBounceAxis) {
		case BounceAxis::X:
			velocity.x *= -1;
			break;
		case BounceAxis::Y:
			velocity.y *= -1;
			break;
		case BounceAxis::XY:
			velocity.x *= -1;
			velocity.y *= -1;
			break;
		}
	}

	PhysicsObject::update(dt);

	prevBounces = bounces;
}

bool Projectile::getCollided() const {
	return collided;
}

unsigned int Projectile::getBounces() const {
	return bounces;
}
unsigned int Projectile::getMaxBounces() const {
	return maxBounces;
}

const SDLGameObject* Projectile::getOwner() {
	return owner;
}

int Projectile::getDamage() const {
	return damage;
}

void Projectile::setBounceAxis(const BounceAxis& axis) {
	lastBounceAxis = axis;
}