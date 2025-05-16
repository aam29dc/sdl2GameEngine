#include "level.hpp"
#include "core/renderer.hpp"
#include "core/camera.hpp"
#include "tiles/tile.hpp"
#include "managers/textureManager.hpp"
#include "objects/SDLGameObject.hpp"
#include "objects/destructible.hpp"
#include "actors/player.hpp"
#include "actors/npc.hpp"
#include "objects/gameItem.hpp"
#include "utils/fileUtils.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

Level::Level(const size_t mapWidth, const size_t mapHeight) : light({ 0,0 }, 15) {
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

	bgColor = { 0, 0, 0, 255 };

	textureID.bg = 0;
	textureID.ground = 0;
	vectorMaxCols = 0;
	textureID.object = 0;
	textureID.item = 0;

	light = Light({ 0, 0 }, 15);
}

Level::~Level() {
	bgTiles.clear();
	groundTiles.clear();

	for (auto* obj : objects) delete obj;
	objects.clear();

	for (auto* item : items) delete item;
	items.clear();
}

// REDUCE CODE REUSE HERE
bool Level::load(
	Renderer* renderer,
	const std::string& bgLevel, const std::string& bgImg,
	const std::string& groundLevel, const std::string& groundImg,
	const std::string& objectsLevel, const std::string& objectsImg,
	const std::string& itemsLevel, const std::string& itemsImg){

	TextureManager* tm = TextureManager::getInstance();
	if (!tm->load(renderer, textureID.bg, bgImg)) return false;
	if (!tm->load(renderer, textureID.ground, groundImg)) return false;
	if (!tm->load(renderer, textureID.object, objectsImg)) return false;
	if (!tm->load(renderer, textureID.item, itemsImg)) return false;

	std::ifstream bgFile(bgLevel);
	if (!bgFile.is_open()) {
		std::cout << "Failed to open: " << bgLevel << "." << std::endl;
		return false;
	}

	char ch = 0;
	int row = 0;
	int col = 0;

	while (bgFile.get(ch)) {
		if ('0' <= ch && 'o' >= ch) {
			bgTiles.emplace_back(Tile(convertToSrc(ch, { Tile::WIDTH, Tile::HEIGHT }, Tile::MARGIN),
				{0, 0 }, false, true)	// bg tiles repeat in sequence
			);
		}
	}

	std::ifstream groundFile(groundLevel);
	if (!groundFile.is_open()) {
		std::cout << "Failed to open: " << groundLevel << "." << std::endl;
		return false;
	}

	ch = 0;
	row = 0;
	col = 0;

	while (groundFile.get(ch)) {
		if ('0' <= ch && 'o' >= ch) {
			groundTiles.emplace_back(Tile(convertToSrc(ch, {Tile::WIDTH, Tile::HEIGHT}, Tile::MARGIN),
				{col * Tile::WIDTH, row * Tile::HEIGHT})
			);
			++col;
		}
		else if (ch == ' ') { // allows our groundTiles vector to correspond to position in world
			groundTiles.push_back(Tile(convertToSrc(ch,	{ Tile::WIDTH, Tile::HEIGHT }, Tile::MARGIN),
				{ col * Tile::WIDTH, row * Tile::HEIGHT },
				false, false)
			);
			++col;
		}
		else if (ch == ',') {
			if (vectorMaxCols <= (unsigned)col) vectorMaxCols = (unsigned)col;	// the grounds file sets our max cols for the other files
			col = 0;
			++row;
		}
	}

	std::ifstream objectsFile(objectsLevel);
	if (!objectsFile.is_open()) {
		std::cout << "Failed to open: " << objectsLevel << "." << std::endl;
	}

	ch = 0;
	row = 0;
	col = 0;

	/* (We could: insert col, row into our objects for indexing) */
	while (objectsFile.get(ch)) {
		if ('0' <= ch && 'o' >= ch) {
			objects.emplace_back(new Destructible(
				textureID.object,
				{ (float)col * SDLGameObject::SIZE, (float)row * SDLGameObject::SIZE },
				convertToSrc(ch, { SDLGameObject::SIZE, SDLGameObject::SIZE }, SDLGameObject::MARGIN),
				true, false)
			);
			++col;
		}
		else if (ch == ' ') {
			++col;
		}
		else if (ch == ',') {
			col = 0;
			++row;
		}
	}

	std::ifstream itemsFile(itemsLevel);
	if (!itemsFile.is_open()) {
		std::cout << "Failed to open: " << itemsLevel << "." << std::endl;
	}

	ch = 0;
	row = 0;
	col = 0;

	/* BUG: currently doesn't get the src for the ch */
	while (itemsFile.get(ch)) {
		if ('0' <= ch && '4' >= ch) {
			items.emplace_back(new GameItem(
				textureID.item,
				{ (float)col * SDLGameObject::SIZE, (float)row * SDLGameObject::SIZE },
				convertToItemSrc(ch, { SDLGameObject::SIZE, SDLGameObject::SIZE }, SDLGameObject::MARGIN),
				(ItemType)(ch - 48))
			);
			++col;
		}
		else if (ch == ' ') {
			++col;
		}
		else if (ch == ',') {
			col = 0;
			++row;
		}
	}

	return true;
}

const Int2 Level::convertToItemSrc(const char ch, const Int2& size, const int margin) const {
	// 0 to 4 (lesser heal, major heal, mana potion, swiftness potion
	// each row a differnt item
	int c = ch - 48;

	int row = c / 8;

	return {1, (size.y * row) + (row * margin) + 1	};
}

const Int2 Level::convertToSrc(const char ch, const Int2& size, const int margin) const {
	// (48) 0 - 9, :, ;, <, =, >, ?, @, A-O (111)
	int c = ch - 48;

	int row = c / 8;
	int col = c % 8;

	return {
		(size.x * col) + (col * margin) + 1,
		(size.y * row) + (row * margin) + 1
	};
}

void Level::drawMap(Renderer* renderer, const Player* player, const Camera& camera, const float scale) const {
	SDL_Rect src = { 0, 0, Tile::WIDTH, Tile::HEIGHT };
	SDL_FRect dst = { 0, 0, Tile::WIDTH * scale, Tile::HEIGHT * scale };

	for (auto it = groundTiles.cbegin(); it != groundTiles.cend(); ++it) {
		src.x = it->getSrc().x;
		src.y = it->getSrc().y;
		dst.x = (float)it->getDst().x * scale;
		dst.y = (float)it->getDst().y * scale;
		dst = camera.worldToView(dst);
		renderer->draw(textureID.ground, src, dst);
	}

	for (auto it = objects.cbegin(); it != objects.cend(); ++it) {
		(*it)->draw(renderer, camera);
	}

	player->draw(renderer, camera);
}

void Level::draw(Renderer* renderer, Player* player, const std::vector<Npc*>& npc, const Camera& camera) {
	SDL_Rect src = { 0, 0, Tile::WIDTH, Tile::HEIGHT };
	SDL_FRect dst = { 0, 0, Tile::WIDTH, Tile::HEIGHT };

	//draw ground first
	for (auto it = groundTiles.cbegin(); it != groundTiles.cend(); ++it) {
		if (!it->getVisible()) continue;	// if invisible skip draw
		src.x = it->getSrc().x;
		src.y = it->getSrc().y;
		dst.x = (float)it->getDst().x;
		dst.y = (float)it->getDst().y;

		dst = camera.worldToView(dst);
		renderer->draw(textureID.ground, src, dst);
	}

	//cast shadows
	light.drawShadows(renderer, objects, camera);

	//add objects, player, npcs, gameitems to one container
	std::vector<SDLGameObject*> sortedObjects = objects;
	sortedObjects.emplace_back(player);
	sortedObjects.insert(sortedObjects.end(), npc.begin(), npc.end());
	for (int i = 0; i < items.size(); ++i) {
		sortedObjects.emplace_back(items[i]);
	}

	// y sort, least to greatest for depth
	std::sort(sortedObjects.begin(), sortedObjects.end(), [](SDLGameObject* a, SDLGameObject* b) {
		return a->getPos().y < b->getPos().y;
		});

	for (auto it = sortedObjects.cbegin(); it != sortedObjects.cend(); ++it) {
		(*it)->draw(renderer, camera);
	}
}

void Level::drawBackground(Renderer* renderer, const Camera& camera) const {
	//repeat sequence of tiles
	SDL_Rect src = { 0, 0, Tile::WIDTH, Tile::HEIGHT };
	SDL_FRect dst = { 0, 0, Tile::WIDTH, Tile::HEIGHT };
	Int2 size = { (int)camera.getCamSize().x, (int)camera.getCamSize().y };

	int index = 0;
	for (int y = 0; y < size.y; y += Tile::HEIGHT) {
		for (int x = 0; x < size.x; x += Tile::WIDTH) {
			if (index >= bgTiles.size()) index = 0;
			src.x = bgTiles[index].getSrc().x;
			src.y = bgTiles[index].getSrc().y;
			dst.x = (float)x;
			dst.y = (float)y;

			renderer->draw(textureID.bg, src, dst);
			index++;
		}
	}
}

bool Level::isWalkable(const Float2& pos) const {
	float tileX = pos.x / Tile::WIDTH;
	float tileY = pos.y / Tile::HEIGHT;

	#ifdef DEBUG
		std::cout << "Player position: (" << pos.x << ", " << pos.y << "), " << "Tile coordinates (float): x:" << tileX << ", y:" << tileY << std::endl;
	#endif

	if (tileX < 0.0f || tileY < 0.0f || tileX >= vectorMaxCols || tileY >= (groundTiles.size() / vectorMaxCols)) {
		return false;
	}

	unsigned int index = static_cast<int>(tileY) * vectorMaxCols + static_cast<int>(tileX);
	if (index >= groundTiles.size()) {
		return false;
	}

	return groundTiles[index].getWalkable();
}

size_t Level::getMapWidth() const {
	return mapWidth;
}

size_t Level::getMapHeight() const {
	return mapHeight;
}

const std::vector<Tile>& Level::getGroundTiles() const {
	return groundTiles;
}

std::vector<SDLGameObject*>& Level::getObjects() {
	return objects;
}

std::vector<GameItem*>& Level::getItems() {
	return items;
}

LevelTextureID Level::getTextureID() const {
	return textureID;
}