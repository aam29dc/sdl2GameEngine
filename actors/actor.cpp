#include "actor.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "objects/meleeAttack.hpp"
#include "objects/projectile.hpp"
#include "managers/soundManager.hpp"
#include "ui/uiElement.hpp"

Actor::Actor(const Float2& pos, UITextBox* const combatlog) : AnimateGameObject(0, pos, {0,0}, false, false),
	weapons({ Weapon("Bow", 0.1f, 200.0f, 4.0f, 20, 3), Weapon("Melee", 0.5f, 10.0f, 32.0f, 20) })
{
	weapons[0].setSfx("sfx_shoot");
	weapons[1].setSfx("sfx_melee");

	textureID = 0;
	actorTextureID.move = 0;
	actorTextureID.attack = 0;
	actorTextureID.moveAttack = 0;

	attackFrameSpeed = 1.0f;

	frameCols = 8;
	frameRows = 8;

	//size is 32x64 for actors
	size = { (float)SIZE, ((float)SIZE * 2) };
	this->isStatic = false;

	health = 100;
	maxHealth = 100;

	currentWeapon = 0;
	lastFireTime = 0.0f;

	angle = 0.0f;

	state = States::IDLE;
	lastState = state;

	isNPC = true;

	this->combatlog = combatlog;
}

Actor::~Actor() {
	if (combatlog) {
		combatlog = nullptr;
	}
}

void Actor::update(const float dt) {
	AnimateGameObject::update(dt);

	unsigned int frameCol = frame % frameCols;
	bool isPlayingAttackAnim = (state == States::ATTACKING || state == States::MOVINGATTACK) && frameCol < frameCols;

	lastState = state;

	// Handle movement
	bool isMoving = std::fabsf(velocity.x * dt) > 0.1f || std::fabsf(velocity.y * dt) > 0.1f;

	if (isPlayingAttackAnim) {
		// Allow switching between attack and moving attack
		if (isMoving && state == States::ATTACKING) state = States::MOVINGATTACK;
		else if (!isMoving && state == States::MOVINGATTACK) state = States::ATTACKING;
	}
	else {
		if (isMoving) state = States::MOVING;
		else state = States::IDLE;
	}

	if (lastState != state && !isPlayingAttackAnim) {
		resetFrame();
	}
}

void Actor::draw(Renderer* renderer, const Camera& camera, const bool& iso) const {
	float angleDeg = angle * (180.0f / (float)M_PI);
	angleDeg = std::fmod(angleDeg + 360.0f, 360.0f);  // Normalize
	angleDeg += 22.5f;  // Center sectors
	if (angleDeg >= 360.0f) angleDeg -= 360.0f;

	unsigned int frameRow = static_cast<unsigned int>(angleDeg / 45.0f);
	unsigned int frameCol = frame % frameCols;

	size_t id = textureID;
	switch (state) {
	case States::IDLE: default:
		id = textureID;
		break;
	case States::ATTACKING:
		id = actorTextureID.attack;
		break;
	case States::MOVING:
		id = actorTextureID.move;
		break;
	case States::MOVINGATTACK:
		id = actorTextureID.moveAttack;
		break;
	}

	SDL_FRect dst = { pos.x, pos.y, (float)size.x, (float)size.y };

	dst = camera.worldToView(dst);

	if (iso) dst = renderer->viewToIso(dst);

	renderer->draw(id, {
			static_cast<int>((frameCol * PhysicsObject::getWidth()) + (frameCol * SDLGameObject::MARGIN) + 1),
			static_cast<int>((frameRow * PhysicsObject::getHeight()) + (frameRow * SDLGameObject::MARGIN) + 1),
			static_cast<int>(size.x),
			static_cast<int>(size.y)
		}
	, dst);
}

void Actor::attack(SoundManager* soundManager, const float now, std::vector<Projectile*>& projectiles, std::vector<MeleeAttack*>& swings) {
	if (now < lastFireTime + weapons[currentWeapon].getRateOfFire()) {
		if (state == States::IDLE) state = States::ATTACKING;
		else if (state == States::MOVING) state = States::MOVINGATTACK;
		return;
	}

	lastFireTime = (float)now;

	//Melee Weapon Attack
	if (weapons[currentWeapon].getIsMelee()) {
		swings.push_back(new MeleeAttack(pos + Float2{ getWidth() / 2.0f, getHeight() / 2.0f }, angle, 0.0f, getWidth(), weapons[currentWeapon].getDamage(), now, this));
	}
	//Projectile Weapon Attack
	else {
		float speed = weapons[currentWeapon].getProjectileSpeed();

		Float2 velocity = {
			speed * std::cosf(angle),
			speed * std::sinf(angle)
		};

		Float2 centerOffset = { getWidth() / 2.0f, getHeight() / 2.0f };
		Float2 spawnOffset = {
			32.0f * std::cosf(angle),
			32.0f * std::sinf(angle)
		};

		Float2 spawnPos = pos + centerOffset + spawnOffset;

		projectiles.push_back(
			new Projectile(
				spawnPos,
				velocity,
				weapons[currentWeapon].getDamage(),
				weapons[currentWeapon].getMaxBounces(),
				weapons[currentWeapon].getProjectileColor(),
				this
			)
		);
	}

	soundManager->push(weapons[currentWeapon].getSfx(), pos);

	//Change State to Attack/MovingAttack
	if (state == States::IDLE) state = States::ATTACKING;
	else if (state == States::MOVING) state = States::MOVINGATTACK;
}

std::queue<Commands>& Actor::getCommands() {
	return commandQueue;
}

void Actor::setState(const States& state) {
	this->state = state;
}
States Actor::getState() const {
	return state;
}

float Actor::getAngle() const {
	return angle;
}

float Actor::getIsNPC() const {
	return isNPC;
}

size_t& Actor::getMoveTextureID() {
	return actorTextureID.move;
}

size_t& Actor::getAttackTextureID() {
	return actorTextureID.attack;
}

size_t& Actor::getMoveAttackTextureID() {
	return actorTextureID.moveAttack;
}