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
		rect.x / vScreen::vWidth * window->getWidth(),
		rect.y / vScreen::vHeight * window->getHeight(),
		rect.w / vScreen::vWidth * window->getWidth(),
		rect.h / vScreen::vHeight * window->getHeight()
	};
}

Float2 Renderer::scale(const Float2& rect) const {
	return {
		rect.x / vScreen::vWidth * window->getWidth(),
		rect.y / vScreen::vHeight * window->getHeight()
	};
}

void Renderer::draw(const size_t id, const SDL_Rect& src, const SDL_FRect& dst) const {
	SDL_FRect fdst = scale(dst);
	SDL_Rect ssrc = { src.x + 1, src.y + 1, src.w - 2, src.h - 2 };	//removes margin bleed
	SDL_RenderCopyF(sdlRenderer, textures->getTexture(id), &ssrc, &fdst);
	if (wire) {
		SDL_SetRenderDrawColor(sdlRenderer, wireColor.r, wireColor.g, wireColor.b, wireColor.a);
		SDL_RenderDrawRectF(sdlRenderer, &fdst);
	}
}