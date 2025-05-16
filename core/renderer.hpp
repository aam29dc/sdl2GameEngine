#pragma once

#include <SDL_render.h>

#include "math/arrayEx.hpp"

class Window;
class TextureManager;

class Renderer {
private:
	static Renderer* instance;
	Window* window;
	TextureManager* textures;

	bool wire = false;
	SDL_Color wireColor = { 255,255, 0, 255 };
public:
	SDL_Renderer* sdlRenderer;

	Renderer();
	~Renderer();

	bool init(Window* window, TextureManager* textures);

	SDL_FRect scale(const SDL_FRect& rect) const;
	Float2 scale(const Float2& rect) const;

	void draw(const size_t id, const SDL_Rect& src, const SDL_FRect& dst) const;
};