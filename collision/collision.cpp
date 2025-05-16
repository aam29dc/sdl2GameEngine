#include "collision.hpp"
#include "objects/destructible.hpp"
#include "objects/meleeAttack.hpp"
#include "objects/projectile.hpp"
#include "actors/player.hpp"
#include "tiles/level.hpp"
#include <cmath>
#include <algorithm>
#include <utility>
#include <iostream>

/* Prone to floating point rounding errors, should implement point as a circle with a small radius */
bool Collision::pointIntersectLineSegment(const Float2& pt, const Float2& tail, const Float2& head) {
    //vertical line
    if (head.x == tail.x) {
        return pt.x == head.x && pt.y >= std::min(head.y, tail.y) && pt.y <= std::max(head.y, tail.y);
    }

    //non-vertical line
    float slope = (head.y - tail.y) / (head.x - tail.x);
    float b = head.y - slope * head.x;

    //range check point on line SEGMENT, then evalute point on the line function
    return (pt.x >= std::min(head.x, tail.x)) && (pt.x <= std::max(head.x, tail.x)) && (pt.y == slope * pt.x + b);
}

bool Collision::pointIntersectRect(const Float2& pt, SDL_FRect& target) {
    return ((pt.x >= target.x) && (pt.x <= target.x + target.w)
        && (pt.y >= target.y) && (pt.y <= target.y + target.h));
}

bool Collision::pointIntersectCircle(const Float2& pt, const Float2& pos, const float rad) {
    float dx = pt.x - pos.x;
    float dy = pt.y - pos.y;
    return (dx * dx) + (dy * dy) <= rad * rad;
}

bool Collision::lineSegmentsIntersect(const Float2& headA, const Float2& tailA, const Float2 headB, const Float2 tailB) {
    Float2 hA = headA;
    Float2 hB = headB;
    Float2 tA = tailA;
    Float2 tB = tailB;

    //one of the lines is vertical, we swap the x and y axis (on all pts), and continue with same algorithm
    if (hA.x == tA.x || hB.x == tB.x) {
        std::swap(hA.x, hA.y);
        std::swap(hB.x, hB.y);
        std::swap(tA.x, tA.y);
        std::swap(tB.x, tB.y);
    }

    //both lines are vertical
    if (hA.x == tA.x && hB.x == tB.x) {
        return std::min(tA.y, hA.y) <= std::max(tB.y, hB.y) //only check intersection on one (y) axis
            && std::max(tA.y, hA.y) >= std::min(tB.y, hB.y);
    }

    float mA = (hA.y - tA.y) / (hA.x - tA.x);
    float mB = (hB.y - tB.y) / (hB.x - tB.x);
    float bA = hA.y - (mA * hA.x);
    float bB = hB.y - (mB * hB.x);
    
    // parallel
    if (mA == mB && bA != bB) return false; //equal slopes, and b's arent equal then they're parallel
    else if (mA == mB && bA == bB) {  //equal slops and equal b's then the line segments are on the same line
        return std::min(tA.x, hA.x) <= std::max(tB.x, hB.x) //only check intersection on one (x) axis
            && std::max(tA.x, hA.x) >= std::min(tB.x, hB.x);
    }

    // non-parallel, thus they intersect at some pt
    // pt of intersection yA = yB <=> mAx+bA = mBx+bB, should be on both line segments
    float x = (bB - bA) / (mA - mB);
    float yA = mA * x + bA;
    //float yB = mB * x + bB; // yA == yB, otherwise don't intersect

    //if the x coord of intersection is <= the min(right sides of the segments)
    //and x coord is >= the max(of the left sides of the segments), then its on both segments
    return x <= std::min(std::max(tA.x, hA.x), std::max(tB.x, hB.x))
        && x >= std::max(std::min(tA.x, hA.x), std::min(tB.x, hB.x));
}

bool Collision::lineSegmentIntersectRect(const Float2& tail, const Float2& head, SDL_FRect& rect) {
    // Helper: check if two line segments intersect
    auto linesIntersect = [](Float2 p1, Float2 p2, Float2 q1, Float2 q2) {
        auto cross = [](Float2 a, Float2 b) {
            return a.x * b.y - a.y * b.x;
            };
        Float2 r = { p2.x - p1.x, p2.y - p1.y };
        Float2 s = { q2.x - q1.x, q2.y - q1.y };
        Float2 qp = { q1.x - p1.x, q1.y - p1.y };

        float rxs = cross(r, s);
        float qpxr = cross(qp, r);

        if (rxs == 0.0f && qpxr == 0.0f) {
            // Collinear
            return false;
        }
        if (rxs == 0.0f) return false; // Parallel

        float t = cross(qp, s) / rxs;
        float u = cross(qp, r) / rxs;

        return (t >= 0 && t <= 1) && (u >= 0 && u <= 1);
        };

    // Rect edges as line segments
    Float2 topLeft = { rect.x, rect.y };
    Float2 topRight = { rect.x + rect.w, rect.y };
    Float2 bottomLeft = { rect.x, rect.y + rect.h };
    Float2 bottomRight = { rect.x + rect.w, rect.y + rect.h };

    // Check all 4 edges
    if (linesIntersect(tail, head, topLeft, topRight)) return true;
    if (linesIntersect(tail, head, topRight, bottomRight)) return true;
    if (linesIntersect(tail, head, bottomRight, bottomLeft)) return true;
    if (linesIntersect(tail, head, bottomLeft, topLeft)) return true;

    return false;
}

/*
bool Collision::lineSegmentIntersectCircle(const Float2& tail, const Float2& head, SDL_FRect& target) {
    //if the square of the diagonal line segment doesn't intersect the rect, then it'll never intersect
    SDL_FRect square = {
        std::min(head.x, tail.x),
        std::min(head.y, tail.y),
        std::fabsf(head.x - tail.x),
        std::fabsf(head.y - tail.y)
    };
    if (!rectsIntersect(square, target)) {
        //std::cout << "sqX: " << square.x << " sqY: " << square.y << "sqW: " << square.w << "sqH: " << square.h << std::endl;
        //std::cout << "rects don't x\n";
        return false;
    }

    //if any of the end points of the line segment intersect, then they intersect
    if (pointIntersectRect(tail, target) || pointIntersectRect(head, target)) return true;

    //cases: (square of diagonal intersects, but end points don't intersect), thus the line intersects the rect on its sides
    //evalute the function using the x values of the rectangle
    float m = (head.y - tail.y) / (head.x - tail.x);
    float b = head.y - m * head.x;    // y = mx + b => b = y - mx
    return pointIntersectRect({ target.x, (m * target.x) + b }, target) || pointIntersectRect({ target.x + target.w, (m * (target.x + target.w)) + b }, target);
}
*/

bool Collision::lineSegmentIntersectCircle(const Float2& tail, const Float2& head, const Float2& pos, const float rad) {
    float dY = head.y - tail.y;
    float dX = head.x - tail.x;


    //vertical line
    if (dX == 0){

    }

    float m = dY / dX;

    //horizontal line
    if (dY == 0) {

    }

    //get length of perpendicular bisector and if its less than rad then they intersect
    float mPerp = -1 / m;

    return false;
}

bool Collision::rectsIntersect(const SDL_FRect& A, const SDL_FRect& B) {
	return A.x <= B.x + B.w && A.x + A.w >= B.x &&
        A.y <= B.y + B.h && A.y + A.h >= B.y;
}

inline bool Collision::circlesIntersect(const Float2& posA, const int radA, const Float2& posB, const int radB) {
	float dx = (posA.x - posB.x);
	float dy = (posA.y - posB.y);

 	return dx*dx + dy*dy <= (radA + radB) * (radA + radB);
}

bool Collision::circleIntersectRect(const Float2& posA, const float radA, const SDL_FRect& B) {
    float closestX = std::clamp(posA.x, B.x, B.x + B.w);
    float closestY = std::clamp(posA.y, B.y, B.y + B.h);

    float dx = posA.x - closestX;
    float dy = posA.y - closestY;

    return (dx * dx + dy * dy) <= (radA * radA);
}

void Collision::projectileBounds(const float dt, std::vector<Projectile*>& projectiles, const Level& level) {
    for (int i = 0; i < projectiles.size(); ++i) {
        if (projectiles[i]->getBounces() >= projectiles[i]->getMaxBounces()) continue;

        Float2 velocity = projectiles[i]->getVelocity() * dt;
        Float2 pos = projectiles[i]->getPos();
        Float2 size = projectiles[i]->getSize();

        // FULL movement check (diagonal/corner case)
        bool fullCollision = !(
            level.isWalkable(pos + velocity) &&
            level.isWalkable(pos + velocity + Float2{size.x, 0}) &&
            level.isWalkable(pos + velocity + Float2{0, size.y}) &&
            level.isWalkable(pos + velocity + size)
            );

        if (!fullCollision) {
            continue; // no collision, continue to next projectile
        }

        // Axis-aligned checks
        bool collidedX = false;
        bool collidedY = false;

        auto nextPosX = pos + Float2{ velocity.x, 0 };
        auto nextPosY = pos + Float2{ 0, velocity.y };

        if (!(level.isWalkable(nextPosX) &&
            level.isWalkable(nextPosX + Float2{size.x, 0}) &&
            level.isWalkable(nextPosX + Float2{0, size.y}) &&
            level.isWalkable(nextPosX + size))) {
            collidedX = true;
        }

        if (!(level.isWalkable(nextPosY) &&
            level.isWalkable(nextPosY + Float2{size.x, 0}) &&
            level.isWalkable(nextPosY + Float2{0, size.y}) &&
            level.isWalkable(nextPosY + size))) {
            collidedY = true;
        }

        if (collidedX || collidedY) {
            projectiles[i]->setBounceAxis(collidedX && collidedY ? BounceAxis::XY : (collidedX ? BounceAxis::X : BounceAxis::Y));
            projectiles[i]->setCollided();
        }
    }
}

void Collision::levelBounds(const float dt, std::vector<Actor*>& actors, const Level& level) {
    for (int i = 0; i < actors.size();++i) {
        auto& velocity = actors[i]->getVelocity();
        auto& pos = actors[i]->getPos();
        auto& size = actors[i]->getSize();

        Float2 newPosX = pos + Float2{ velocity.x * dt, 0.0f };
        Float2 newPosY = pos + Float2{ 0.0f, velocity.y * dt };

        // Check horizontal movement
        if (!(level.isWalkable(newPosX) &&
            level.isWalkable(newPosX + Float2{ size.x, 0.0f }) &&
            level.isWalkable(newPosX + Float2{ 0.0f, size.y }) &&
            level.isWalkable(newPosX + size)))
        {
            actors[i]->setVelocityX(0.0f);
        }

        // Check vertical movement
        if (!(level.isWalkable(newPosY) &&
            level.isWalkable(newPosY + Float2{ size.x, 0.0f }) &&
            level.isWalkable(newPosY + Float2{ 0.0f, size.y }) &&
            level.isWalkable(newPosY + size)))
        {
            actors[i]->setVelocityY(0.0f);
        }
    }
}

void Collision::gridBounds(const float dt, Player& player, const Level& level) {
    auto velocity = player.getVelocity() * dt;

    if (player.getPos().x + velocity.x <= 0 || player.getPos().x + player.getWidth() + velocity.x >= level.getMapWidth()) {
        player.setVelocityX(0);
    }

    if (player.getPos().y + velocity.y <= 0 || player.getPos().y + player.getHeight() + velocity.y >= level.getMapHeight()) {
        player.setVelocityY(0);
    }
}

bool Collision::checkLineOfSight(const Float2& origin, const Float2& target, const float losRange, std::vector<SDLGameObject*>& objects) {
    float distTarget = ((target.y - origin.y) * (target.y - origin.y) + (target.x - origin.x) * (target.x - origin.x));

    if (distTarget > losRange*losRange) return false;

    float distObject = 0;
    Float2 objPos = { 0,0 };
    SDL_FRect objFRect = { 0 };

    //if any object interects the line of sight before the target, then the target isnt los
    for (int i = 0; i < objects.size();++i) {
        if (objects[i]->getIsPlayer()) continue;

        objPos = objects[i]->getPos();
        distObject = ((objPos.y - origin.y) * (objPos.y - origin.y) + (objPos.x - origin.x) * (objPos.x - origin.x));
        if (distObject > distTarget || distObject == 0) continue;   //if distObject == 0 then target maybe itself (in objects list)

        objFRect = arrayToFRect(objects[i]->getPos(), objects[i]->getSize());
        if (lineSegmentIntersectRect(origin, target, objFRect)) return false;
    }
    return true;
}

void Collision::checkProjectileCollisions(const float dt, std::vector<Projectile*>& projectiles, std::vector<SDLGameObject*>& objects) {
    // Broad-phase sort to optimize
    std::sort(objects.begin(), objects.end(), [](SDLGameObject* a, SDLGameObject* b) { return a->getPos().x < b->getPos().x; });
    std::sort(projectiles.begin(), projectiles.end(), [](Projectile* a, Projectile* b) { return a->getPos().x < b->getPos().x; });

    for (size_t i = 0; i < projectiles.size(); ++i) {
        if (projectiles[i]->getBounces() >= projectiles[i]->getMaxBounces()) continue;

        for (size_t j = 0; j < objects.size(); ++j) {
            if (projectiles[i]->getOwner() == objects[j]) continue; // 983

            // Early out from sort
            if (projectiles[i]->getPos().x + projectiles[i]->getWidth() < objects[j]->getPos().x) break;

            SDL_FRect A = arrayToFRect(projectiles[i]->getPos() + (projectiles[i]->getVelocity() * dt), projectiles[i]->getSize());
            SDL_FRect B = arrayToFRect(objects[j]->getPos() + (objects[j]->getVelocity() * dt), objects[j]->getSize());

            if (!rectsIntersect(A, B)) continue;

            // Axis-based bounce resolution
            bool collidedX = false;
            bool collidedY = false;

            auto nextPosX = projectiles[i]->getPos() + Float2{ projectiles[i]->getVelocity().x * dt, 0 };
            SDL_FRect AX = arrayToFRect(nextPosX, projectiles[i]->getSize());
            if (rectsIntersect(AX, B)) {
                collidedX = true;
            }

            auto nextPosY = projectiles[i]->getPos() + Float2{ 0, projectiles[i]->getVelocity().y * dt };
            SDL_FRect AY = arrayToFRect(nextPosY, projectiles[i]->getSize());
            if (rectsIntersect(AY, B)) {
                collidedY = true;
            }

            if (collidedX || collidedY) {
                projectiles[i]->setBounceAxis(collidedX && collidedY ? BounceAxis::XY : (collidedX ? BounceAxis::X : BounceAxis::Y));
                projectiles[i]->setCollided();

                if (Destructible* destructible = dynamic_cast<Destructible*>(objects[j])) {
                    destructible->takeDamage(projectiles[i]->getDamage());
                }
                else if (Actor* actor = dynamic_cast<Actor*>(objects[j])) {
                    actor->takeDamage(projectiles[i]->getDamage());
                }

                break; // break from object loop once a bounce occurs
            }
        }
    }
}

void Collision::checkMeleeCollisions(const float dt, std::vector<MeleeAttack*>& swings, std::vector<SDLGameObject*>& objects) {
    // Broad-phase sort to optimize
    std::sort(objects.begin(), objects.end(), [](SDLGameObject* a, SDLGameObject* b) {
        return a->getPos().x < b->getPos().x;
        });

    std::sort(swings.begin(), swings.end(), [](MeleeAttack* a, MeleeAttack* b) {
        return a->getPos().x < b->getPos().y;
        });

    //Melee Attacks only Register hits in ONE frame, then whats left over is a draw frame
    for (size_t i = 0; i < swings.size(); ++i) {
        if (!swings[i]->getCanHit()) continue;
        for (size_t j = 0; j < objects.size(); ++j) {
            if (swings[i]->getOwner() == objects[j]) continue;

            // Early out from sort
            if (swings[i]->getPos().x + swings[i]->getRadius() < objects[j]->getPos().x) break;

            SDL_FRect B = arrayToFRect(objects[j]->getPos() + objects[j]->getVelocity() * dt, objects[j]->getSize());

            if (circleIntersectRect(swings[i]->getPos(), swings[i]->getRadius(), B)) {
                if (Destructible* destructible = dynamic_cast<Destructible*>(objects[j])) {
                    destructible->takeDamage(swings[i]->getDamage());
                }
            }
        }
        swings[i]->disableHit();    //disable hitcheck after checking against all objects
    }
}

/*
    BUG: PLAYER 'FLICKERS' between MOVING AND IDLE WHEN MOVING TOWARDS COLLIDING OBJECT (NOT WALLS)
*/
void Collision::checkObjectCollisions(const float dt, std::vector<SDLGameObject*>& objects) {
    // Sort objects by X to optimize broad-phase collision checks
    std::sort(objects.begin(), objects.end(), [](SDLGameObject* a, SDLGameObject* b){ return a->getPos().x < b->getPos().x;});

    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            // Early out: if object i is too far left of j, stop checking further (due to sort)
            if (objects[i]->getPos().x + objects[i]->getWidth() < objects[j]->getPos().x) break;

            // Skip if both objects are static (non-moving objects should never collide)
            if (objects[i]->getStatic() && objects[j]->getStatic()) continue;

            // Predict future positions using velocity
            SDL_FRect A = arrayToFRect(objects[i]->getPos() + objects[i]->getVelocity() * dt, objects[i]->getSize());
            SDL_FRect B = arrayToFRect(objects[j]->getPos() + objects[j]->getVelocity() * dt, objects[j]->getSize());

            if (rectsIntersect(A, B)) {
                // Calculate the overlap amounts
                float deltaX = (A.x + A.w / 2.0f) - (B.x + B.w / 2.0f);
                float overlapAmtX = (A.w + B.w) / 2.0f - std::abs(deltaX);

                float deltaY = (A.y + A.h / 2.0f) - (B.y + B.h / 2.0f);
                float overlapAmtY = (A.h + B.h) / 2.0f - std::abs(deltaY);

                // Resolve the collision based on the smaller overlap
                if (overlapAmtX < overlapAmtY) {
                    // Resolve X-axis collision
                    if (!objects[i]->getStatic()) {
                        objects[i]->setVelocityX(0);
                        // Move player to the edge of the object along the X-axis
                        if (deltaX > 0) {
                            // Moving right, adjust the player's X position
                            objects[i]->setPos(Float2{ objects[i]->getPos().x + overlapAmtX, objects[i]->getPos().y });
                        }
                        else {
                            // Moving left, adjust the player's X position
                            objects[i]->setPos(Float2{ objects[i]->getPos().x - overlapAmtX, objects[i]->getPos().y });
                        }
                    }
                    if (!objects[j]->getStatic()) {
                        objects[j]->setVelocityX(0);
                        // Move object to the edge of the player along the X-axis
                        if (deltaX > 0) {
                            objects[j]->setPos(Float2{ objects[j]->getPos().x - overlapAmtX, objects[j]->getPos().y });
                        }
                        else {
                            objects[j]->setPos(Float2{ objects[j]->getPos().x + overlapAmtX, objects[j]->getPos().y });
                        }
                    }
                }
                else {
                    // Resolve Y-axis collision
                    if (!objects[i]->getStatic()) {
                        objects[i]->setVelocityY(0);
                        // Move player to the edge of the object along the Y-axis
                        if (deltaY > 0) {
                            // Moving down, adjust the player's Y position
                            objects[i]->setPos(Float2{ objects[i]->getPos().x, objects[i]->getPos().y + overlapAmtY });
                        }
                        else {
                            // Moving up, adjust the player's Y position
                            objects[i]->setPos(Float2{ objects[i]->getPos().x, objects[i]->getPos().y - overlapAmtY });
                        }
                    }
                    if (!objects[j]->getStatic()) {
                        objects[j]->setVelocityY(0);
                        // Move object to the edge of the player along the Y-axis
                        if (deltaY > 0) {
                            objects[j]->setPos(Float2{ objects[j]->getPos().x, objects[j]->getPos().y - overlapAmtY });
                        }
                        else {
                            objects[j]->setPos(Float2{ objects[j]->getPos().x, objects[j]->getPos().y + overlapAmtY });
                        }
                    }
                }
                #ifdef DEBUG
                    std::cout << "Collision (axis-based) between i: " << i << " and j: " << j << "\n";
                #endif
            }
        }
    }
}
