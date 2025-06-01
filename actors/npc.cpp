#include "npc.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "actors/player.hpp"
#include "ui/uiElement.hpp"

Npc::Npc(const Float2& pos, UITextBox* const combatlog) : Actor(pos, combatlog)
{
	weapons = { Weapon("Bow", 0.5f, 200.0f, 4.0f, 15, 1), Weapon("Melee", 0.5f, 10.0f, 32.0f, 20) };
	weapons[0].setProjectileColor({ 255, 255/4, 0, 255 });
	weapons[0].setSfx("sfx_shoot");
	weapons[1].setSfx("sfx_melee");
	origin = pos;
	aggroRadius = 600.0f;
	maxRadius = 100.0f;
	patrolRadius = 96.0f;
	patrolDir = 1;
	stopTime = 3.0f;
	lastStopTime = 0.0f;
	attackRadius = 100.0f;

	lastPlayerLOS = false;
}

Npc::~Npc() {

}

void Npc::update(const float dt, const float now, const Player& player, const bool playerLOS, const Camera& camera) {
	Actor::update(dt);

	if (playerLOS) {
		Float2 toPlayer = { player.getPos().x - pos.x, player.getPos().y - pos.y };
		float dist = (toPlayer.x * toPlayer.x) + (toPlayer.y * toPlayer.y);

		angle = atan2f(toPlayer.y, toPlayer.x);

		float angleDeg = angle * (180.0f / (float)M_PI);
		angleDeg = std::fmod(angleDeg + 360.0f, 360.0f);
		if (angleDeg >= 360.0f) angleDeg -= 360.0f;

		if (dist > maxRadius * maxRadius) {
			if (angleDeg == 0) {
				commandQueue.push(Commands::RIGHTWARD);
			}
			else if (angleDeg > 0 && angleDeg < 90) {
				commandQueue.push(Commands::FORWARD);
				commandQueue.push(Commands::RIGHTWARD);
			}
			else if (angleDeg == 90) {
				commandQueue.push(Commands::FORWARD);
			}
			else if (angleDeg > 90 && angleDeg < 180) {
				commandQueue.push(Commands::FORWARD);
				commandQueue.push(Commands::LEFTWARD);
			}
			else if (angleDeg == 180) {
				commandQueue.push(Commands::LEFTWARD);
			}
			else if (angleDeg > 180 && angleDeg < 270) {
				commandQueue.push(Commands::BACKWARD);
				commandQueue.push(Commands::LEFTWARD);
			}
			else if (angleDeg == 270) {
				commandQueue.push(Commands::BACKWARD);
			}
			else if (angleDeg > 270 && angleDeg < 360) {
				commandQueue.push(Commands::BACKWARD);
				commandQueue.push(Commands::RIGHTWARD);
			}
		}

		commandQueue.push(Commands::ATTACKONE);
	}

	/*else if (!playerLOS && lastPlayerLOS) {

	}
	else if(!playerLOS && !lastPlayerLOS) { //patrol
		//future feature: add a random # of ms to stopTime
		*/

	else{
		if (now - lastStopTime >= stopTime) {
			if (patrolDir == 1) {
				if (pos.x < origin.x + patrolRadius) {
					commandQueue.push(Commands::RIGHTWARD);
				}
				else {
					patrolDir = -1;
					angle = (float)M_PI;
					lastStopTime = now;
				}
			}
			else {
				if (pos.x > origin.x) {
					commandQueue.push(Commands::LEFTWARD);
				}
				else {
					patrolDir = 1;
					angle = 0;
					lastStopTime = now;
				}
			}
		}
	}

	lastPlayerLOS = playerLOS;
}

void Npc::drawLOS(Renderer* renderer, const Player& player, const Camera& camera) const {
	Float2 newPos = renderer->scale(camera.worldToView(pos));
	Float2 playerPos = renderer->scale(camera.worldToView(player.getPos()));


	SDL_SetRenderDrawColor(renderer->sdlRenderer, 255, 0, 0, 255);
	SDL_RenderDrawLineF(renderer->sdlRenderer, newPos.x, newPos.y, playerPos.x, playerPos.y);
}

void Npc::setPos(const Float2& pos) {
	PhysicsObject::setPos(pos);
	origin = pos;
}

float Npc::getLosRadius() const {
	return aggroRadius;
}

void Npc::takeDamage(const int damage) {
	health -= damage;
	combatlog->addLine("You did " + std::to_string(damage) + " damage to Enemy.");
}