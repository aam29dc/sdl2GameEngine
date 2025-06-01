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
#include "utils/levelUtils.hpp"
#include "enums/color.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

Level::Level(const size_t mapWidth, const size_t mapHeight) : light({ 0,0 }, 15) {
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

	bgColor = Color::Black;

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
	const std::string& itemsLevel, const std::string& itemsImg, const bool& iso){

	TextureManager* tm = TextureManager::getInstance();
	if (bgLevel!="" && !tm->load(renderer, textureID.bg, bgImg)) return false;
	if (groundLevel!="" && !tm->load(renderer, textureID.ground, groundImg)) return false;
	if (objectsLevel!="" && !tm->load(renderer, textureID.object, objectsImg)) return false;
	if (itemsLevel!="" && !tm->load(renderer, textureID.item, itemsImg)) return false;

	int tileWidth = Tile::WIDTH;
	int tileHeight = Tile::HEIGHT;

	if (iso) {
		tileWidth = Tile::ISOWIDTH;
		tileHeight = Tile::ISOHEIGHT;
	}

	char ch = 0;
	int row = 0;
	int col = 0;

	if (bgLevel != "") {
		std::ifstream bgFile(bgLevel);
		if (!bgFile.is_open()) {
			std::cout << "Failed to open: " << bgLevel << "." << std::endl;
			return false;
		}

		while (bgFile.get(ch)) {
			if ('0' <= ch && 'o' >= ch) {
				bgTiles.emplace_back(Tile(convertToSrc(ch, { tileWidth, tileHeight }, Tile::MARGIN),
					{ 0, 0 }, false, true)	// bg tiles repeat in sequence
				);
			}
		}
	}

	if (groundLevel != "") {
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
				groundTiles.emplace_back(Tile(convertToSrc(ch, { tileWidth, tileHeight }, Tile::MARGIN),
					{ (float)col * Tile::WIDTH, (float)row * Tile::HEIGHT })
				);
				++col;
			}
			else if (ch == ' ') { // allows our groundTiles vector to correspond to position in world
				groundTiles.push_back(Tile(convertToSrc(ch, { tileWidth, tileHeight }, Tile::MARGIN),
					{ (float)col * Tile::WIDTH, (float)row * Tile::HEIGHT },
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
	}

	if (objectsLevel != "") {
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
	}

	if (itemsLevel != "") {
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
	}

	return true;
}

/*
	BUG: ground tiles don't properly render when zoomed in/out
*/

void Level::draw(Renderer* renderer, Player* player, const std::vector<Npc*>& npc, const Camera& camera, const bool& iso, const float& angle) {
	int tileWidth = Tile::WIDTH;
	int tileHeight = Tile::HEIGHT;

	if (iso) {
		tileWidth = Tile::ISOWIDTH;
		tileHeight = Tile::ISOHEIGHT;
	}

	SDL_Rect src = { 0, 0, tileWidth, tileHeight };
	SDL_FRect dst = { 0, 0, static_cast<float>(Tile::WIDTH), static_cast<float>(Tile::HEIGHT) };

	//draw ground first
	for (auto it = groundTiles.cbegin(); it != groundTiles.cend(); ++it) {
		if (!it->getVisible()) continue;	// if invisible skip draw
		src.x = it->getSrc().x;
		src.y = it->getSrc().y;
		dst.x = static_cast<float>(it->getDst().x);
		dst.y = static_cast<float>(it->getDst().y);

		dst = camera.worldToView(dst);

		if (iso) dst = renderer->viewToIso(dst);

		renderer->draw(textureID.ground, src, dst, false, angle*180.0f/(float)M_PI);
	}

	//cast shadows
	light.drawShadows(renderer, objects, camera, iso);

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
		(*it)->draw(renderer, camera, iso);
	}
}

/*
	FIX: iso render
*/
void Level::drawBackground(Renderer* renderer, const Camera& camera, const bool& iso) const {
	int tileWidth = Tile::WIDTH;
	int tileHeight = Tile::HEIGHT;

	if (iso) {
		tileWidth = Tile::ISOWIDTH;
		tileHeight = Tile::ISOHEIGHT;
	}
	//repeat sequence of tiles
	SDL_Rect src = { 0, 0, tileWidth, tileHeight };
	SDL_FRect dst = { 0, 0, (float)tileWidth, (float)tileHeight };
	Int2 size = { (int)camera.getCamSize().x, (int)camera.getCamSize().y };

	if (bgTiles.size() <= 0) return;

	int index = 0;
	for (int y = 0; y < size.y; y += tileHeight) {
		for (int x = 0; x < size.x; x += tileWidth) {
			if (index >= bgTiles.size()) index = 0;
			src.x = bgTiles[index].getSrc().x;
			src.y = bgTiles[index].getSrc().y;
			dst.x = (float)x;
			dst.y = (float)y;

			if (iso) dst = renderer->viewToIso(dst);
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