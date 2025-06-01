#pragma once

#include "objects/light.hpp"
#include <string>
#include <vector>
#include <tuple>	// ?

class Renderer;
class Npc;
class Player;
class Tile;
class SDLGameObject;
class GameItem;
class Camera;
class Light;

struct LevelTextureID {
	size_t bg;
	size_t ground;
	size_t object;
	size_t item;
};

class Level {
private:
	std::vector<Tile> bgTiles;
	std::vector<Tile> groundTiles;
	unsigned int vectorMaxCols;

	SDL_Color bgColor;

	LevelTextureID textureID;

	std::vector<SDLGameObject*> objects;
	std::vector<GameItem*> items;

	Light light;

	size_t mapWidth;
	size_t mapHeight;
public:
	Level(const size_t mapWidth = 0, const size_t mapHeight = 0);
	~Level();

	bool load(Renderer* renderer,
		const std::string& bgLevel = "", const std::string& bgImg = "",
		const std::string& groundLevel = "", const std::string& groundImg = "",
		const std::string& objectsLevel = "", const std::string& objectsImg = "",
		const std::string& itemsLevel = "", const std::string& itemsImg = "", const bool& iso = false);

	void draw(Renderer* renderer, Player* player, const std::vector<Npc*>& npc, const Camera& camera, const bool& iso = false, const float& angle = 0.0f);
	void drawBackground(Renderer* renderer, const Camera& camera, const bool& iso = false) const;

	bool isWalkable(const Float2& pos) const;

	size_t getMapWidth() const;
	size_t getMapHeight() const;

	const std::vector<Tile>& getGroundTiles() const;

	std::vector<SDLGameObject*>& getObjects();
	std::vector<GameItem*>& getItems();

	LevelTextureID getTextureID() const;
};