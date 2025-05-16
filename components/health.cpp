#include "components/health.hpp"

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