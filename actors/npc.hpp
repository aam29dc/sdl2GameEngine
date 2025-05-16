#pragma once

#include "actors/actor.hpp"

class Renderer;
class Camera;
class Player;

enum class NpcStates {
	AGGRESIVE,
	PATROL,
	PASSIVE,
};

class Npc : public Actor {
private:
	float aggroRadius;
	float maxRadius;

	int patrolDir;
	Float2 origin;
	float patrolRadius;

	float stopTime;
	float lastStopTime;
	float attackRadius;

	bool lastPlayerLOS;
public:
	Npc(const Float2& pos = { 0.0f, 0.0f });
	~Npc();

	void update(const float dt, const float now, const Player& player, const bool playerLOS, const Camera& camera);
	void drawLOS(Renderer* renderer, const Player& player, const Camera& camera) const;
	void setPos(const Float2& pos);
	float getLosRadius() const;
};