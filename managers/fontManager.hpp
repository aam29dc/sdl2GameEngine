#pragma once

#include <SDL_ttf.h>
#include <string>

#include "enums/virtualScreen.hpp"

class Renderer;

class FontManager {
public:
	enum class FontSize {
		SMALL = 1,
		MEDIUM = 2,
		LARGE = 3
	};

	static constexpr int TEXTSIZE_SMALL = 12;
	static constexpr int TEXTSIZE_MEDIUM = 17;
	static constexpr int TEXTSIZE_LARGE = 22;
	static constexpr int FONTWIDTH = 13;

	static FontManager& getInstance();

	bool init(const std::string& fontPath = "assets/fonts/RobotoMono-VariableFont_wght.ttf");
	void shutdown();

	void drawText(Renderer* renderer, const std::string& str, const FontSize size,
		const float xpos, const float ypos,
		const SDL_Color& color,
		unsigned int WindowWidth = vScreen::vWidth, const unsigned int WindowHeight = vScreen::vHeight,
		const unsigned maxtext = 0, const bool limit = false);

private:
	FontManager() = default;
	~FontManager();

	FontManager(const FontManager&) = delete;
	FontManager& operator=(const FontManager&) = delete;

	TTF_Font* fontSmall = nullptr;
	TTF_Font* fontMedium = nullptr;
	TTF_Font* fontLarge = nullptr;

	TTF_Font* getFont(FontSize size);
};