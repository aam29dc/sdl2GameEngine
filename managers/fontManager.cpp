#include "FontManager.hpp"
#include "core/renderer.hpp"
#include <memory>

#ifdef DEBUG
	#include <iostream>
#endif

FontManager& FontManager::getInstance() {
	static FontManager instance;
	return instance;
}

bool FontManager::init(const std::string& fontPath) {
	if (TTF_Init() == -1) {
#ifdef DEBUG
		std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
#endif
		return false;
	}

	fontLarge = TTF_OpenFont(fontPath.c_str(), TEXTSIZE_LARGE);
	fontMedium = TTF_OpenFont(fontPath.c_str(), TEXTSIZE_MEDIUM);
	fontSmall = TTF_OpenFont(fontPath.c_str(), TEXTSIZE_SMALL);

	if (!fontLarge || !fontMedium || !fontSmall) {
#ifdef DEBUG
		std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
#endif
		return false;
	}
	return true;
}

void FontManager::shutdown() {
	if (fontLarge) TTF_CloseFont(fontLarge);
	if (fontMedium) TTF_CloseFont(fontMedium);
	if (fontSmall) TTF_CloseFont(fontSmall);
	fontLarge = fontMedium = fontSmall = nullptr;
	TTF_Quit();
}

FontManager::~FontManager() {
	shutdown();
}

TTF_Font* FontManager::getFont(FontSize size) {
	switch (size) {
	case FontSize::LARGE:  return fontLarge;
	case FontSize::MEDIUM: return fontMedium;
	case FontSize::SMALL:  return fontSmall;
	default: return nullptr;
	}
}

void FontManager::drawText(Renderer* renderer, const std::string& str, const FontSize size,
	const float xpos, const float ypos, const SDL_Color& color, const unsigned int WindowWidth, const unsigned int WindowHeight,
	const unsigned maxtext, const bool limit) {

	if (str.empty()) return;

	TTF_Font* font = getFont(size);
	if (!font) return;

	SDL_Surface* surfaceMsg = TTF_RenderText_Solid(font, str.c_str(), color);
	if (!surfaceMsg) {
		#ifdef DEBUG
				std::cerr << "Text render error: " << TTF_GetError() << "\n";
		#endif
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->sdlRenderer, surfaceMsg);
	if (!texture) {
		#ifdef DEBUG
				std::cerr << "Texture creation error: " << SDL_GetError() << "\n";
		#endif
		SDL_FreeSurface(surfaceMsg);
		return;
	}

	SDL_FRect dstRect = { xpos, ypos - 4, (float)surfaceMsg->w, (float)surfaceMsg->h };
	SDL_Rect srcRect = { 0, 0, surfaceMsg->w, surfaceMsg->h };

	if (limit && str.length() > maxtext) {
		srcRect.w = FONTWIDTH * maxtext;
		dstRect.w = (float)srcRect.w;
		srcRect.x = static_cast<int>((str.length() - maxtext) * FONTWIDTH);
	}

	dstRect = {
		dstRect.x,
		dstRect.y,
		dstRect.w / vScreen::vWidth * WindowWidth,
		dstRect.h / vScreen::vHeight * WindowHeight
	};

	SDL_RenderCopyF(renderer->sdlRenderer, texture, &srcRect, &dstRect);

	SDL_FreeSurface(surfaceMsg);
	SDL_DestroyTexture(texture);
}