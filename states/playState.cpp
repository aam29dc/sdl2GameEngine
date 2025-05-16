#include "playState.hpp"
#include "states/gameStateMachine.hpp"
#include "states/pauseState.hpp"
#include "states/gameOverState.hpp"
#include "tiles/level.hpp"
#include "actors/player.hpp"
#include "actors/npc.hpp"
#include "objects/destructible.hpp"
#include "objects/projectile.hpp"
#include "objects/meleeAttack.hpp"
#include "objects/gameItem.hpp"
#include "core/camera.hpp"
#include "core/time.hpp"
#include "collision/collision.hpp"
#include "managers/textureManager.hpp"
#include "managers/soundManager.hpp"
#include "managers/saveManager.hpp"
#include "ui/userInterface.hpp"
#include <algorithm>

PlayState::PlayState(Window* window, GameStateMachine* GSM) : GameState(window, GSM, "Play") {
	level1 = nullptr;
	player = nullptr;
	camera = new Camera({ 1440.0f, 900.0f });
	timer = 0.0f;
}

PlayState::~PlayState() {
	//All Objects in level (objects, gameitems)
	if (level1) delete level1;
	level1 = nullptr;

	//Actors
	if (player) delete player;
	player = nullptr;
	for (auto* npc : npcs) {
		delete npc;
	}

	//Projectiles
	for (auto* projectile : projectiles) {
		delete projectile;
	}

	//Swings
	for (auto* swing : swings) {
		delete swing;
	}

	allObjects.clear();
	actors.clear();
	npcs.clear();
	projectiles.clear();
	swings.clear();
}

void PlayState::handleEvents() {
	UI->handleEvents();
	if(player) player->input(Input::getInputHandler()->getMousePosition());

	if (Input::getInputHandler()->isKeyReleased(SDL_SCANCODE_ESCAPE)) {
		GSM->queuePush(new PauseState(window, GSM));
	}

	if (Input::getInputHandler()->isKeyReleased(SDL_SCANCODE_EQUALS)) {
		camera->setZoom(camera->getZoom() * 2.0f);
	}

	if (Input::getInputHandler()->isKeyReleased(SDL_SCANCODE_MINUS)) {
		camera->setZoom(camera->getZoom() * 0.5f);
	}

	if (Input::getInputHandler()->isKeyReleased(SDL_SCANCODE_H)) {
		//SaveManager::saveToFile(makeSnapShot(), "assets/saves/save.json");
		clear();
		return;
	}
}

void PlayState::update(SoundManager* soundManager) {
	const float dt = Time::getInstance()->getDt();
	timer += dt;
	const float now = (float)Time::getInstance()->getElapsedTime();
	UI->update();

	// [UPDATE] actors
	for (auto* actor : actors) {
		actor->setVelocity({ 0,0 });	//RESET velocity to 0 so when player presses forward, player doesn't always keep going forward
		//POLL the command queue of each player, & bots
		std::queue<Commands>& cmds = actor->getCommands();
		while (!cmds.empty()) {
			Commands cmd = cmds.front();
			cmds.pop();
			float speed = actor->getSpeed();
			switch (cmd) {
			case Commands::FORWARD:    actor->setVelocityY(speed); break;
			case Commands::BACKWARD:   actor->setVelocityY(-speed); break;
			case Commands::LEFTWARD:   actor->setVelocityX(-speed); break;
			case Commands::RIGHTWARD:  actor->setVelocityX(speed); break;
			case Commands::ATTACKONE:
				actor->attack(soundManager, now, projectiles, swings);
				break;
			default: break;
			}
		}
	}

	//CHECK COLLISIONS
	Collision::checkObjectCollisions(dt, allObjects);
	Collision::checkProjectileCollisions(dt, projectiles, allObjects);
	Collision::checkMeleeCollisions(dt, swings, allObjects);
	Collision::levelBounds(dt, actors, *level1);
	Collision::projectileBounds(dt, projectiles, *level1);

	//GAMEOVER on DEAD Player
	if (player->isDead()) {
		GSM->queuePush(new GameOverState(window, GSM));
	}

	// [REMOVE]
	//REMOVE ALL melee attacks that passed their drawtime
	for (auto it = swings.begin(); it != swings.end();) {
		if (now - (*it)->getStartTime() > MeleeAttack::drawTime) {
			delete* it;
			it = swings.erase(it);
		}
		else ++it;
	}
	bool change = false;
	// REMOVE destroyed level objects (no delete, just remove)
	if (level1) {
		for (auto it = level1->getObjects().begin(); it != level1->getObjects().end(); ) {
			if (Destructible* destructible = dynamic_cast<Destructible*>(*it)) {
				if (destructible->isDead()) {
					it = level1->getObjects().erase(it); // NO delete here
					change = true;
					continue;
				}
			}
			++it;
		}
	}

	//REMOVE dead Npcs
	for (auto it = npcs.begin(); it != npcs.end(); ) {
		if ((*it)->isDead()) {
			delete* it;
			it = npcs.erase(it);
			change = true;
		}
		else ++it;
	}

	if (change) {
		refreshActors();
		refreshAllObjects();
	}

	//REMOVE collided projectiles, destroyed objects
	for (auto it = projectiles.begin(); it != projectiles.end(); ) {
		if ((*it)->getBounces() >= (*it)->getMaxBounces()) {
			delete* it;
			it = projectiles.erase(it); // erase handles advancing the iterator
		}
		else ++it; // advance only if not erased
	}

	// [UPDATE]
	//UPDATE all our projectiles & actors
	for (auto* projectile : projectiles) {
		projectile->update(dt);
	}
	if(player) player->update(dt, *camera);

	//update camera position
	if (camera && player) {
		camera->follow(player->getPos());

		// CALC LOS for each npc
		for (auto* npc : npcs) {
			bool LOS = Collision::checkLineOfSight(npc->getPos(), player->getPos(), npc->getLosRadius(), level1->getObjects());
			npc->update(dt, now, *player, LOS, *camera);
		}
	}

	//update gameitems
	auto& items = level1->getItems();
	for (auto& item : items) {
		item->update(dt);
	}

	//PLAYER ITEM PICKUP (AFTER Position update)
	if (player) {
		Float2 playerPos = player->getPos();	// + player->getVelocity() * dt; // this is BEFORE applying pos update
		for (auto* item : level1->getItems()) {
			if (Collision::rectsIntersect(
				arrayToFRect(playerPos, player->getSize()),
				arrayToFRect(item->getPos(), { (float)SDLGameObject::SIZE, (float)SDLGameObject::SIZE })
			)) {
				if (player->pickupItem(item->getType())) {
					item->setPickedUp();
				}
			}
		}
		//DELETE PICKED UP ITEMS
		for (auto it = level1->getItems().begin(); it != level1->getItems().end();) {
			if ((*it)->getPickedUp()) {
				delete* it;
				it = level1->getItems().erase(it);
			}
			else it++;
		}
	}
}

void PlayState::refreshActors() {
	actors.clear();
	actors.emplace_back(player);
	actors.insert(actors.end(), npcs.begin(), npcs.end());
}

void PlayState::refreshAllObjects() {
	auto& levelObjs = level1->getObjects();
	allObjects.clear();
	allObjects.insert(allObjects.end(), levelObjs.begin(), levelObjs.end());
	allObjects.emplace_back(player);
	allObjects.insert(allObjects.end(), npcs.begin(), npcs.end());
}

void PlayState::render(Renderer* renderer) const {
	if (level1) {
		level1->drawBackground(renderer, *camera);
		//draw tiles, objects, items, and entities with y-mapping
		if(player) level1->draw(renderer, player, npcs, *camera);
	}

	//draw projectiles
	// (this should be y-sorted as well)
	for (auto* projectile : projectiles) {
		projectile->draw(renderer, *camera);
	}
	//draw melee swings
	for (auto* swing : swings) {
		swing->draw(renderer, *camera);
	}

	/*SDL_Rect miniMapViewport = {screenWidth - 150, 10, 140, 140};
	SDL_RenderSetViewport(renderer, &miniMapViewport);
	level1->drawMap(renderer, player, screenWidth, screenHeight, 0.2f);*/

	UI->render(renderer);
}

void PlayState::loadTextures(Renderer* renderer) {
	//Load Player Frame textures
	TextureManager* tm = TextureManager::getInstance();
	tm->load(renderer, player->getTextureID(), "assets/textures/playerIdleFrame.png");
	tm->load(renderer, player->getMoveTextureID(), "assets/textures/playerMoveFrame.png");
	tm->load(renderer, player->getAttackTextureID(), "assets/textures/playerAttackFrame.png");
	tm->load(renderer, player->getMoveAttackTextureID(), "assets/textures/playerMoveAttackFrame.png");

	//Load NPC Frame textures
	size_t npcIdleTextureID = 0, npcMoveTextureID = 0, npcAttackTextureID = 0, npcMoveAttackTextureID = 0;

	tm->load(renderer, npcIdleTextureID, "assets/textures/npcIdleFrame.png");
	tm->load(renderer, npcMoveTextureID, "assets/textures/npcMoveFrame.png");
	tm->load(renderer, npcAttackTextureID, "assets/textures/npcAttackFrame.png");
	tm->load(renderer, npcMoveAttackTextureID, "assets/textures/npcMoveAttackFrame.png");
	for (auto* npc : npcs) {
		npc->getTextureID() = npcIdleTextureID;
		npc->getMoveTextureID() = npcMoveTextureID;
		npc->getAttackTextureID() = npcAttackTextureID;
		npc->getMoveAttackTextureID() = npcMoveAttackTextureID;
	}
}

// REDUCE CODE REUSE HERE
bool PlayState::onEnter(Renderer* renderer, SoundManager* soundManager) {
	std::cout << "Entering play state" << std::endl;

	//Allocate player
	player = new Player({ 200.0f, 600.0f });
	camera->follow(player->getPos());

	//Allocate npcs
	npcs.clear();
	npcs.emplace_back(new Npc());
	npcs[0]->setPos({ 64, 192 });

	//Create UI Elements

	UI->addElement(new UIResource<Player, int>({ 10, 10, 100, 20 }, { 0,255,0,255 }, false, player, &Player::getHealth, &Player::getMaxHealth));
	UI->addElement(new UIText<Player, float>({ 10, 40, 140, 20 }, { 0,255,0,255 }, false, player, { "X: ", " Y: " }, { &Player::getPosX, &Player::getPosY }));
	UI->addElement(new UIText<PlayState, float>({ 670, 10, 100, 20 }, { 0,255,0,255 }, false, this,	{ "Time: " }, { &PlayState::getTimer }));
	UI->addElement(new UIText<Time, float>({ 770, 10, 100, 20 }, { 0,255,0,255 }, false, Time::getInstance(), { "FPS: " }, { &Time::getDt }, { [](float time) -> float { return time != 0.0f ? 1.0f / time : 0.0f; } }));

	//Load level
	level1 = new Level(800, 800);
	level1->load(renderer,
		"assets/level/level1.bg", "assets/textures/bg.png",
		"assets/level/level1.grd", "assets/textures/ground.png",
		"assets/level/level1.objs", "assets/textures/objects.png",
		"assets/level/level1.items", "assets/textures/items.png");

	loadTextures(renderer);

	//Allocate actors and all objects containers
	refreshActors();
	refreshAllObjects();

	//Load sounds
	soundManager->load("assets/sounds/shoot.wav", "sfx_shoot", soundType::SOUND_SFX);
	soundManager->load("assets/sounds/click.wav", "sfx_click", soundType::SOUND_SFX);
	soundManager->load("assets/sounds/melee.wav", "sfx_melee", soundType::SOUND_SFX);
	soundManager->load("assets/sounds/gameOver.wav", "sfx_gameOver", soundType::SOUND_SFX);
	soundManager->load("assets/sounds/potionUse.wav", "sfx_potionUse", soundType::SOUND_SFX);

	return true;
}

bool PlayState::onExit(SoundManager* soundManager) {
	std::cout << "Exiting play state\n";
	TextureManager::getInstance()->clear();
	soundManager->clear();
	return true;
}

void PlayState::clear() {
	if (level1) {
		delete level1;
		level1 = nullptr;
	}

	allObjects.clear();

	for (auto* a : actors) delete a;
	actors.clear();
	npcs.clear();
	player = nullptr;

	for (auto* p : projectiles)	delete p;
	projectiles.clear();

	for (auto* s : swings) delete s;
	swings.clear();

	timer = 0;
}

bool PlayState::loadSave(const std::string& path) {
	std::optional<SaveData> s = SaveManager::loadFromFile(path);

	if (s.has_value()) {
		for (auto& a : s->actors) {
			if (a.type == "Player") {
				player = new Player(a.pos);
				player->setHealth(a.health);
				player->setVelocity(a.velocity);
			}
			else if (a.type == "NPC") {
				npcs.push_back(new Npc(a.pos));
				npcs.back()->setHealth(a.health);
				npcs.back()->setVelocity(a.velocity);
			}
		}

		for (auto& o : s->objects) {
			level1->getObjects().push_back(new SDLGameObject(0, o.pos, o.src));
		}

		for (auto& d : s->destructibles) {
			level1->getObjects().push_back(new Destructible(0, d.pos, d.src));
		}

		for (auto& i : s->items) {
			level1->getItems().push_back(new GameItem(0, i.pos, i.src, i.type));
		}
		return true;
	}

	return false;
}

SaveData PlayState::makeSnapShot() const {
	SaveData s;
	s.level = "level1";
	s.timer = timer;
	s.actors.push_back({ "Player", player->getPos(), player->getVelocity(), player->getHealth(), player->getAngle() });
	for (auto* npc : npcs) {
		s.actors.push_back({ "NPC", npc->getPos(), npc->getVelocity(), npc->getHealth(), npc->getAngle() });
	}

	for (auto* i : level1->getItems()) {
		s.items.push_back({ i->getType(), i->getPos(), i->getPickedUp(), i->getSrcPos() });
	}

	for (auto* o : level1->getObjects()) {
		if (auto* od = dynamic_cast<Destructible*>(o)) {
			s.destructibles.push_back({ od->getPos(), od->getVelocity(), od->getHealth(), od->getSrcPos()});
		}
		else {
			s.objects.push_back({ o->getPos(), o->getVelocity(), o->getSrcPos() });
		}
	}

	return s;
}

Camera* PlayState::getCamera() const {
	return camera;
}