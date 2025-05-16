#include "weapons/weapon.hpp"

Weapon::Weapon() {
	name = "NULL";
	rateOfFire = 0;
	damage = 0;

	swingSpeed = 0;
	radius = 0;
	isMelee = 0;

	projectileSpeed = 0;
	projectileSize = 0;
	maxBounces = 0;

	projectileColor = { 255, 255, 0, 175 };

	sfx = "NULL";
}

Weapon::Weapon(const std::string& name, const float rateOfFire, const float projectileSpeed, const float projectileSize, const int damage, unsigned int maxBounces)
	: name(name), rateOfFire(rateOfFire), projectileSpeed(projectileSpeed), projectileSize(projectileSize), damage(damage), maxBounces(maxBounces) {
	isMelee = false;

	swingSpeed = 0.0f;
	radius = 0.0f;

	projectileColor = { 255, 255, 0, 175 };

	sfx = "NULL";
}

Weapon::Weapon(const std::string& name, const float rateOfFire, const float swingSpeed, const float radius, const int damage)
: name(name), rateOfFire(rateOfFire), swingSpeed(swingSpeed), radius(radius), damage(damage){
	isMelee = true;

	projectileSpeed = 0.0f;
	projectileSize = 0.0f;
	maxBounces = 0;

	projectileColor = { 255, 255, 0, 175 };

	sfx = "NULL";
}

Weapon::~Weapon() {}

const std::string& Weapon::getName() const {
	return name;
}
float Weapon::getRateOfFire() const {
	return rateOfFire;
}

bool Weapon::getIsMelee() const {
	return isMelee;
}

int Weapon::getDamage() const {
	return damage;
}

float Weapon::getSwingSpeed() const {
	return swingSpeed;
}

float Weapon::getRadius() const {
	return radius;
}

void Weapon::setProjectileColor(const SDL_Color& color) {
	this->projectileColor = color;
}

const SDL_Color& Weapon::getProjectileColor() const {
	return projectileColor;
}

float Weapon::getProjectileSpeed() const {
	return projectileSpeed;
}

float Weapon::getProjectileSize() const {
	return projectileSize;
}

unsigned int Weapon::getMaxBounces() const {
	return maxBounces;
}

const std::string& Weapon::getSfx() const {
	return sfx;
}

void Weapon::setSfx(const std::string& sfx) {
	this->sfx = sfx;
}