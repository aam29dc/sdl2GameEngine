#pragma once

#include <SDL_pixels.h>

namespace Color {
	constexpr SDL_Color Red = { 255,0,0,255 };
	constexpr SDL_Color Yellow = { 255,255,0,255 };
	constexpr SDL_Color Green = { 0,255,0,255 };
	constexpr SDL_Color Cyan = { 0,255,255,255 };
	constexpr SDL_Color Blue = { 0,0,255,255 };
	constexpr SDL_Color Purple = { 255,0,255,255 };
	constexpr SDL_Color White = { 255,255,255,255 };
	constexpr SDL_Color Black = { 0,0,0,255 };
	constexpr SDL_Color Shade = { 0,0,0,127 };
	constexpr SDL_Color Light = { 192,192,192,255 };
	constexpr SDL_Color Gray = { 127,127,127,255 };
	constexpr SDL_Color Dark = { 85,85,85,255 };
	constexpr SDL_Color None = { 0,0,0,0 };
}