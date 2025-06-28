#pragma once

class Health {
protected:
	int health;
	int maxHealth;

	float regenPercent = 0.05f;
	float regenTick = 3.0f;
	float timeLastRegen = 0.0f;
public:
	virtual ~Health() {}
	virtual void takeDamage(const int damage);
	bool isDead() const;

	void setHealth(const int health);
	void setMaxHealth(const int health);

	int getMaxHealth() const;
	int getHealth() const;

	void regenHealth(const float dt);
};