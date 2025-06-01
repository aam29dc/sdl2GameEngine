#pragma once

class Health {
protected:
	int health;
	int maxHealth;
public:
	virtual ~Health() {}
	virtual void takeDamage(const int damage);
	bool isDead() const;

	void setHealth(const int health);
	void setMaxHealth(const int health);

	int getMaxHealth() const;
	int getHealth() const;
};