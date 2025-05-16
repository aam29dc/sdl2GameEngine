#include "player.hpp"
#include "core/input.hpp"
#include "core/camera.hpp"

Player::Player(const Float2& pos) : Actor(pos),
	inventory({ ItemType::Empty })
{
	weapons = { Weapon("Bow", 0.1f, 450.0f, 4.0f, 25, 3), Weapon("Melee", 0.5f, 10.0f, 32.0f, 20) };
	weapons[0].setProjectileColor({ 255, 255, 0, 175 });

	weapons[0].setSfx("sfx_shoot");
	weapons[1].setSfx("sfx_melee");

	exp = 0;
	mousePos = { 0 };

	isNPC = false;
	isPlayer = true;
}

Player::~Player() {

}

void Player::update(const float dt, const Camera& camera) {
	Actor::update(dt);
	angle = posToAngle(mousePos, camera);
}

bool Player::pickupItem(const ItemType item) {
	//check storage before pickup
	size_t space = 0;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].getType() != ItemType::Empty) ++space;
	}

	if (space >= inventoryCapacity) return false;

	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].getType() == ItemType::Empty) {
			inventory[i] = item;
			break;
		}
	}

	return true;
}

bool Player::useItem(const size_t item) {
	switch (inventory.at(item).getType()) {
	case ItemType::Empty:
	default:
		return false;
	case ItemType::LesserHeal:
		if (health < maxHealth) {
			health = std::min(maxHealth, health + ItemEffects::LesserHeal);
			inventory[item] = InventoryItem();
			return true;
		}
		break;
	case ItemType::Heal:
		if (health < maxHealth) {
			health = std::min(maxHealth, health + ItemEffects::Heal);
			inventory[item] = InventoryItem();
			return true;
		}
		break;
	case ItemType::Mana:
		return false;
	case ItemType::Speed:
		return false;
	}

	return false;
}

const SDL_FPoint& Player::getMousePos() const {
	return mousePos;
}

float Player::worldPosToAngle(const SDL_FPoint& worldMousePos) const {
	float dx = worldMousePos.x - (pos.x + size.x / 2.0f);
	float dy = worldMousePos.y - (pos.y + size.y / 2.0f);

	return std::atan2(dy, dx);
}

float Player::posToAngle(const SDL_FPoint& mouseScreenPos, const Camera& camera) const {
	// Get center of player in world space
	Float2 playerWorldCenter = {
		pos.x + size.x / 2.0f,
		pos.y + size.y / 2.0f
	};

	// Convert player center to screen space
	Float2 playerScreenCenter = camera.worldToView(playerWorldCenter);

	// Calculate difference between mouse and player in screen space
	float dx = mouseScreenPos.x - playerScreenCenter.x;
	float dy = mouseScreenPos.y - playerScreenCenter.y;

	return std::atan2(dy, dx);
}

/*
	Gather player input (as binds), interpret the binds according to commands, then send to player update
*/
void Player::input(const SDL_FPoint& mousePos) {
	/* for now there is permanent binds	*/

	if (Input::getInputHandler()->isKeyDown(SDL_SCANCODE_W)) {
		commandQueue.push(Commands::FORWARD);
	}
	if (Input::getInputHandler()->isKeyDown(SDL_SCANCODE_S)) {
		commandQueue.push(Commands::BACKWARD);
	}

	if (Input::getInputHandler()->isKeyDown(SDL_SCANCODE_A)) {
		commandQueue.push(Commands::LEFTWARD);
	}
	if (Input::getInputHandler()->isKeyDown(SDL_SCANCODE_D)) {
		commandQueue.push(Commands::RIGHTWARD);
	}

	if (Input::getInputHandler()->getMouseButtonState(LEFT)) {
		commandQueue.push(Commands::ATTACKONE);
	}
	
	this->mousePos = mousePos;
}

void Player::setExp(const unsigned int exp) {
	this->exp = exp;
}

unsigned int Player::getExp() const {
	return exp;
}