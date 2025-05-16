#include "physicsObject.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"

PhysicsObject::PhysicsObject(const Float2& pos, const Float2& velocity, const Float2& size) {
	this->pos = pos;
	this->velocity = velocity;
	speed = 80.0f;
	this->size = size;

	drawFilled = true;
	color = { 255, 255, 0, 255 };
	instanceCount++;
}

PhysicsObject::~PhysicsObject() {

}

size_t PhysicsObject::getInstanceCount() {
	return instanceCount;
}

void PhysicsObject::draw(Renderer* renderer, const Camera& camera) const {
	SDL_FRect rect = { pos.x, pos.y, size.x, size.y };
	SDL_FRect outlineRect = { rect.x - 1, rect.y - 1, rect.w + 2, rect.h + 2 };
	outlineRect = renderer->scale(camera.worldToView(outlineRect));
	rect = renderer->scale(camera.worldToView(rect));

	SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 255/4);
	SDL_RenderDrawRectF(renderer->sdlRenderer, &outlineRect);

	SDL_SetRenderDrawColor(renderer->sdlRenderer, color.r, color.g, color.b, color.a);
	if (drawFilled) SDL_RenderFillRectF(renderer->sdlRenderer, &rect);
	else SDL_RenderDrawRectF(renderer->sdlRenderer, &rect);
}

void PhysicsObject::update(const float dt) {
	pos.x += (float)velocity.x * dt;
	pos.y += (float)velocity.y * dt;
}

void PhysicsObject::clean() {

}

const Float2& PhysicsObject::getSize() const {
	return size;
}

float PhysicsObject::getWidth() const {
	return size.x;
}

float PhysicsObject::getHeight() const {
	return size.y;
}

const Float2& PhysicsObject::getPos() const {
	return pos;
}

float PhysicsObject::getPosX() const {
	return pos.x;
}

float PhysicsObject::getPosY() const {
	return pos.y;
}

const Float2& PhysicsObject::getVelocity() const {
	return velocity;
}

void PhysicsObject::setPos(const Float2& pos) {
	this->pos = pos;
}

void PhysicsObject::setPosX(const float posX) {
	pos.x = posX;
}

void PhysicsObject::setPosY(const float posY) {
	pos.y = posY;
}

void PhysicsObject::setVelocity(const Float2& v) {
	this->velocity = v;
}

void PhysicsObject::setVelocityX(const float velX) {
	velocity.x = velX;
}

void PhysicsObject::setVelocityY(const float velY) {
	velocity.y = velY;
}

float PhysicsObject::getSpeed() const {
	return speed;
}