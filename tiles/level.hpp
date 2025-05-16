#pragma once

#include "objects/light.hpp"
#include <string>
#include <vector>
#include <tuple>

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

	static const int MARGIN = 1;
public:
	Level(const size_t mapWidth, const size_t mapHeight);
	~Level();

	bool load(Renderer* renderer,
		const std::string& bgLevel, const std::string& bgImg,
		const std::string& groundLevel, const std::string& groundImg,
		const std::string& objectsLevel, const std::string& objectsImg,
		const std::string& itemsLevel, const std::string& itemsImg);

	void draw(Renderer* renderer, Player* player, const std::vector<Npc*>& npc, const Camera& camera);
	void drawMap(Renderer* renderer, const Player* player, const Camera& camera, const float scale) const;
	void drawBackground(Renderer* renderer, const Camera& camera) const;

	const Int2 convertToSrc(const char ch, const Int2& size, const int margin) const;
	const Int2 convertToItemSrc(const char ch, const Int2& size, const int margin) const;

	size_t getMapWidth() const;
	size_t getMapHeight() const;

	const std::vector<Tile>& getGroundTiles() const;

	bool isWalkable(const Float2& pos) const;

	std::vector<SDLGameObject*>& getObjects();
	std::vector<GameItem*>& getItems();

	LevelTextureID getTextureID() const;
};