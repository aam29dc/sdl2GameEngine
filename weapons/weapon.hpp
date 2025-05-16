#pragma once

#include "math/arrayEx.hpp"
#include <SDL_pixels.h>
#include <string>

class Weapon {
private:
protected:
	std::string name;
	float rateOfFire;
	int damage;

	float swingSpeed;
	float radius;
	bool isMelee;

	SDL_Color projectileColor;
	float projectileSpeed;
	float projectileSize;
	unsigned int maxBounces;

	std::string sfx;
public:
	Weapon();
	Weapon(const std::string& name, const float rateOfFire, const float projectileSpeed, const float projectileSize, const int damage, unsigned int maxBounces);
	Weapon(const std::string& name, const float rateOfFire, const float swingSpeed, const float radius, const int damage);
	virtual ~Weapon();

	const std::string& getName() const;
	float getRateOfFire() const;
	int getDamage() const;

	bool getIsMelee() const;

	float getSwingSpeed() const;
	float getRadius() const;

	void setProjectileColor(const SDL_Color& color);
	const SDL_Color& getProjectileColor() const;

	float getProjectileSpeed() const;
	float getProjectileSize() const;
	unsigned int getMaxBounces() const;

	const std::string& getSfx() const;
	void setSfx(const std::string& sfx);
};