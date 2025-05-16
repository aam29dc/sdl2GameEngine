#include "textureManager.hpp"
#include "core/renderer.hpp"
#include <iostream>

TextureManager* TextureManager::instance = nullptr;

TextureManager::~TextureManager() {
	clear();

	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

void TextureManager::clear() {
	for (SDL_Texture* tex : textureMap) {
		if (tex != nullptr) {
			SDL_DestroyTexture(tex);
		}
	}
	textureMap.clear();
}

TextureManager* TextureManager::getInstance() {
	if (instance == nullptr) {
		instance = new TextureManager();
	}
	return instance;
}

bool TextureManager::load(Renderer* renderer, size_t& id, const std::string& imgFile) {

	SDL_Texture* tex = IMG_LoadTexture(renderer->sdlRenderer, imgFile.c_str());

	if (tex == nullptr) {
		std::cout << "Texture " << imgFile << " failed to load. " << IMG_GetError() << "\n";
		return false;
	}

	textureMap.push_back(tex);
	id = textureMap.size() - 1;

	return true;
}

bool TextureManager::remove(const size_t& id) {
	if (id < textureMap.size()) {
		if (textureMap.at(id)) {
			SDL_DestroyTexture(textureMap.at(id));
			textureMap.at(id) = nullptr;
			return true;
		}
	}
	return false;
}

SDL_Texture* TextureManager::getTexture(const size_t id) const {
	if (id >= textureMap.size()) return nullptr;
	return textureMap[id];
}