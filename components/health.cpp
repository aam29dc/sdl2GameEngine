#include "components/health.hpp"
#include <algorithm>

void Health::takeDamage(const int damage) {
	health -= damage;
}

bool Health::isDead() const {
	return health < 0;
}

void Health::setHealth(const int health) {
	this->health = health;
}

void Health::setMaxHealth(const int health) {
	maxHealth = health;
}

int Health::getMaxHealth() const {
	return maxHealth;
}

int Health::getHealth() const {
	return health;
}

void Health::regenHealth(const float dt) {
	if (health >= maxHealth) return;

	timeLastRegen += dt;

	if (timeLastRegen >= regenTick) {
		health = std::min(health + int(maxHealth * regenPercent), maxHealth);
		timeLastRegen = 0;
	}
}