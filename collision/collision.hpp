#pragma once

#include "math/arrayEx.hpp"
#include <vector>

class Level;
class SDLGameObject;
class Actor;
class Player;
class Projectile;
class MeleeAttack;

namespace Physics {
	inline constexpr float GRAVITY = 10.0f;
}

class Collision {
public:
	static bool pointIntersectLineSegment(const Float2& pt, const Float2& tail, const Float2& head);
	static bool pointIntersectRect(const Float2& pt, SDL_FRect& target);

	static bool pointIntersectCircle(const Float2& pt, const Float2& pos, const float rad);

	static bool lineSegmentsIntersect(const Float2& headA, const Float2& tailA, const Float2 headB, const Float2 tailB);
	static bool lineSegmentIntersectRect(const Float2& tail, const Float2& head, SDL_FRect& target);
	static bool lineSegmentIntersectCircle(const Float2& tail, const Float2& head, const Float2& pos, const float rad);

	static bool rectsIntersect(const SDL_FRect & A, const SDL_FRect & B);

	static inline bool circlesIntersect(const Float2& posA, const int radA, const Float2& posB, const int radB);
	static bool circleIntersectRect(const Float2& posA, const float radA, const SDL_FRect& B);

	static bool checkLineOfSight(const Float2& origin, const Float2& target, const float losRange, std::vector<SDLGameObject*>& object);

	static void levelBounds(const float dt, std::vector<Actor*>& actors, const Level& level);
	static void checkObjectCollisions(const float dt, std::vector<SDLGameObject*>& objects);

	static void projectileBounds(const float dt, std::vector<Projectile*>& projectiles, const Level& level);
	static void checkProjectileCollisions(const float dt, std::vector<Projectile*>& projectiles, std::vector<SDLGameObject*>& objects);

	static void checkMeleeCollisions(const float dt, std::vector<MeleeAttack*>& swings, std::vector<SDLGameObject*>& objects);

	static void gridBounds(const float dt, Player& player, const Level& level);
};