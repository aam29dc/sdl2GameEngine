#include "renderer.hpp"
#include "core/window.hpp"
#include "managers/textureManager.hpp"
#include "enums/virtualScreen.hpp"

Renderer::Renderer() {
	sdlRenderer = nullptr;
	this->window = nullptr;
	this->textures = nullptr;
}

Renderer::~Renderer() {

}

bool Renderer::init(Window* window, TextureManager* textures) {
	this->window = window;
	sdlRenderer = SDL_CreateRenderer(window->_window, -1, SDL_RENDERER_ACCELERATED);
	this->textures = textures;

	return sdlRenderer != nullptr;
}

SDL_FRect Renderer::scale(const SDL_FRect& rect) const {
	return {
		(float)rect.x / (float)vScreen::vWidth * (float)window->getWidth(),
		(float)rect.y / (float)vScreen::vHeight * (float)window->getHeight(),
		(float)rect.w / (float)vScreen::vWidth * (float)window->getWidth(),
		(float)rect.h / (float)vScreen::vHeight * (float)window->getHeight()
	};
}

Float2 Renderer::scale(const Float2& pt) const {
	return {
		(float)pt.x / (float)vScreen::vWidth * (float)window->getWidth(),
		(float)pt.y / (float)vScreen::vHeight * (float)window->getHeight()
	};
}

Float2 Renderer::viewToIso(const Float2& pt) const {
	return {
		(pt.x - pt.y) * 0.5f,
		(pt.x + pt.y) * 0.25f
	};
}

SDL_FRect Renderer::viewToIso(const SDL_FRect& rect) const {
	return {
		(rect.x - rect.y) * 0.5f,
		(rect.x + rect.y) * 0.25f,
		rect.w,
		rect.h
	};
}

Float2 Renderer::rotate(const Float2& pt, const float& angle) const {
	/*
		r = dist from player to object,
		player rotates in positive => object rotates in negative
	*/
	return {
		pt.x * cosf(angle) - pt.y * sinf(angle),
		pt.x * sinf(angle) + pt.y * cosf(angle)
	};
}

SDL_FRect Renderer::rotate(const SDL_FRect& rect, const float& angle) const {
	return {
		rect.x * cosf(angle) - rect.y * sinf(angle),
		rect.x * sinf(angle) + rect.y * cosf(angle)
	};
}

void Renderer::draw(const size_t id, const SDL_Rect& src, const SDL_FRect& dst, const bool& iso, const float& angle) const {
	SDL_FRect fdst = scale(dst);
	SDL_Rect ssrc = { src.x + 1, src.y + 1, src.w - 2, src.h - 2 };	//removes margin bleed

	//SDL_RenderCopyF(sdlRenderer, textures->getTexture(id), &ssrc, &fdst);
	SDL_RenderCopyExF(sdlRenderer, textures->getTexture(id), &ssrc, &fdst, (double)angle, NULL, SDL_FLIP_NONE);
	if (wire) {
		SDL_SetRenderDrawColor(sdlRenderer, wireColor.r, wireColor.g, wireColor.b, wireColor.a);
		SDL_RenderDrawRectF(sdlRenderer, &fdst);
	}
}