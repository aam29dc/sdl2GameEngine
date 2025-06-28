#pragma once

#include "objects/animateGameObject.hpp"
#include "components/health.hpp"
#include "enums/actorStates.hpp"
#include "weapons/weapon.hpp"
#include <queue>
#include <array>

class Renderer;
class SoundManager;
class Camera;
class MeleeAttack;
class Projectile;
class UITextBox;

struct ActorTextureID {
	size_t move;
	size_t attack;
	size_t moveAttack;
};

class Actor : public AnimateGameObject, public Health {
protected:
	std::queue<Commands> commandQueue;

	std::array<Weapon, 2> weapons;
	size_t currentWeapon;

	float angle;
	float lastFireTime;

	States state;
	States lastState;

	ActorTextureID actorTextureID;
	float attackFrameSpeed;

	bool isNPC;

	UITextBox* combatlog;

	float damageTakenMultiplier;
	float damageDealtMultiplier;
public:
	Actor(const Float2& pos, UITextBox* const combatlog);
	~Actor();

	virtual void update(const float dt);
	virtual void draw(Renderer* renderer, const Camera& camera, const bool& iso = false) const;
	virtual void attack(SoundManager* soundManager, const float now, std::vector<Projectile*>& projectiles, std::vector<MeleeAttack*>& swings);

	void setState(const States& state);

	States getState() const;
	float getAngle() const;
	float getIsNPC() const;
	std::queue<Commands>& getCommands();
	size_t& getMoveTextureID();
	size_t& getAttackTextureID();
	size_t& getMoveAttackTextureID();

	void setDamageTakenMultiplier(const float m);
	float getDamageTakenMultiplier() const;

	void setDamageDealtMultiplier(const float m);
	float getDamageDealtMultiplier() const;

	void takeDamage(const int damage);
};