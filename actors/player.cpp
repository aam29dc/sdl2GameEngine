#include "player.hpp"
#include "core/input.hpp"
#include "core/camera.hpp"
#include "ui/uiElement.hpp"
#include "core/binds.hpp"

Player::Player(const Float2& pos, UITextBox* const combatlog, Binds* const binds, UIConsole* const console) : Actor(pos, combatlog),
	inventory({ ItemType::Empty })
{
	weapons = { Weapon("Bow", 0.1f, 450.0f, 4.0f, 25, 2), Weapon("Melee", 0.5f, 10.0f, 32.0f, 20) };
	weapons[0].setProjectileColor({ 255, 255, 0, 175 });

	weapons[0].setSfx("sfx_shoot");
	weapons[1].setSfx("sfx_melee");

	exp = 0;
	mousePos = { 0 };

	isNPC = false;
	isPlayer = true;

	speedTime = 0.0f;
	speedActive = false;

	this->binds = binds;
	this->console = console;
}

Player::~Player() {

}

void Player::update(const float dt, const Camera& camera) {
	if (speedActive && speedTime < ItemEffects::SpeedDuration) speedTime += dt;
	else {
		speed = MOVESPEED;
		speedActive = false;
	}

	Actor::update(dt);
	angle = posToAngle(mousePos, camera);
}

int Player::pickupItem(const ItemType item) {
	//check storage before pickup
	size_t space = 0;
	for (int i = 0; i < inventory.size(); ++i) {
		if (inventory[i].getType() != ItemType::Empty) ++space;
	}

	if (space >= inventoryCapacity) return -1;

	int index = 0;
	for (; index < inventory.size(); ++index) {
		if (inventory[index].getType() == ItemType::Empty) {
			inventory[index] = item;
			if (combatlog) {
				combatlog->addLine("You picked up a " + to_string(item) + ".");
			}
			break;
		}
	}

	return index;
}

bool Player::useItem(const size_t item) {
	bool usedItem = false;
	ItemType type = ItemType::Empty;

	switch (inventory.at(item).getType()) {
	case ItemType::LesserHeal:
		if (health < maxHealth) {
			health = std::min(maxHealth, health + ItemEffects::LesserHeal);
			inventory[item] = InventoryItem();
			type = ItemType::LesserHeal;
			usedItem = true;
		}
		break;
	case ItemType::Heal:
		if (health < maxHealth) {
			health = std::min(maxHealth, health + ItemEffects::Heal);
			inventory[item] = InventoryItem();
			type = ItemType::Heal;
			usedItem = true;
		}
		break;
	case ItemType::Mana:
		return false;
	case ItemType::Speed:
		if (!speedActive) {
			speed *= ItemEffects::SpeedMultiplier;
			speedTime = 0.0f;
			speedActive = true;
			inventory[item] = InventoryItem();
			type = ItemType::Speed;
			usedItem = true;
		}
		break;
	case ItemType::Empty: default:
		std::cout << "used empty.\n";
		return false;
	}

	if (usedItem) {
		combatlog->addLine("You used a " + to_string(type) + ".");
		return true;
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

void Player::input(const SDL_FPoint& mousePos) {
	if (!console->getVisible()) {
		if (Input::getInputHandler()->isKeyDown(binds->getMoveBind(Movement::FORWARD).key)) {
			commandQueue.push(Commands::FORWARD);
		}
		if (Input::getInputHandler()->isKeyDown(binds->getMoveBind(Movement::BACKWARD).key)) {
			commandQueue.push(Commands::BACKWARD);
		}
		if (Input::getInputHandler()->isKeyDown(binds->getMoveBind(Movement::LEFTWARD).key)) {
			commandQueue.push(Commands::LEFTWARD);
		}
		if (Input::getInputHandler()->isKeyDown(binds->getMoveBind(Movement::RIGHTWARD).key)) {
			commandQueue.push(Commands::RIGHTWARD);
		}
		if (Input::getInputHandler()->getMouseButtonState(binds->getCombatBind(Combat::ATTACKONE).mouseButton)) {
			commandQueue.push(Commands::ATTACKONE);
		}
	}
	
	this->mousePos = mousePos;
}

void Player::setExp(const unsigned int exp) {
	this->exp = exp;
}

unsigned int Player::getExp() const {
	return exp;
}

void Player::takeDamage(const int damage) {
	Actor::takeDamage(damage);
	combatlog->addLine("You took " + std::to_string(damage) + " damage.");
}