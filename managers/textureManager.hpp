#pragma once

#include <SDL_image.h>

#include "math/arrayEx.hpp"

#include <string>
#include <vector>

class Renderer;

class TextureManager {
private:
	static TextureManager* instance;
	std::vector<SDL_Texture*> textureMap;
	static const unsigned int marginW = 1;
	static const unsigned int marginH = 1;
public:
	TextureManager() {}
	~TextureManager();

	static TextureManager* getInstance();

	bool load(Renderer* renderer, size_t& id, const std::string& imgFile);
	bool remove(const size_t& id);
	void clear();
	SDL_Texture* getTexture(const size_t id) const;
};