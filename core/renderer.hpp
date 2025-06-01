#pragma once

#include <SDL_render.h>

#include "math/arrayEx.hpp"
#include "enums/color.hpp"

class Window;
class TextureManager;

class Renderer {
private:
	static Renderer* instance;
	Window* window;
	TextureManager* textures;

	bool wire = false;
	SDL_Color wireColor = Color::Yellow;
public:
	SDL_Renderer* sdlRenderer;

	Renderer();
	~Renderer();

	bool init(Window* window, TextureManager* textures);

	SDL_FRect scale(const SDL_FRect& rect) const;
	Float2 scale(const Float2& pt) const;

	Float2 viewToIso(const Float2& rect) const;
	SDL_FRect viewToIso(const SDL_FRect& rect) const;

	Float2 rotate(const Float2& pt, const float& angle) const;
	SDL_FRect rotate(const SDL_FRect& rect, const float& angle) const;

	void draw(const size_t id, const SDL_Rect& src, const SDL_FRect& dst, const bool& iso = false, const float& angle = 0) const;
};