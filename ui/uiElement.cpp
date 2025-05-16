#include "uiElement.hpp"
#include "core/renderer.hpp"

SDL_FRect UIElement::getScaledPos(const SDL_FRect& rect, const unsigned int windowWidth, const unsigned int windowHeight) const {
	SDL_FRect newRect = rect;

	float scaleX = static_cast<float>(windowWidth) / vScreen::vWidth;
	float scaleY = static_cast<float>(windowHeight) / vScreen::vHeight;

	newRect.x *= scaleX;
	newRect.y *= scaleY;
	newRect.w *= scaleX;
	newRect.h *= scaleY;

	if (centered) {
		newRect.x += (windowWidth - newRect.w) / 2.0f;
		newRect.y += (windowHeight - newRect.h) / 2.0f;
	}

	return newRect;
}

UIElement::UIElement(const SDL_FRect& rect, const SDL_Color& color, const float centered) {
	this->rect = rect;
	this->color = color;
	this->defaultColor = color;
	this->outlineColor = { 0,0,0,255 };
	this->textColor = { 255,0,0,255 };
	this->centered = centered;
	this->visible = true;
}

void UIElement::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	SDL_FRect newRect = getScaledPos(rect, windowWidth, windowHeight);
	SDL_FRect outlineRec = newRect;

	outlineRec.x--;
	outlineRec.y--;
	outlineRec.w += 2;
	outlineRec.h += 2;

	SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 85);
	SDL_RenderFillRectF(renderer->sdlRenderer, &newRect);

	SDL_SetRenderDrawColor(renderer->sdlRenderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
	SDL_RenderDrawRectF(renderer->sdlRenderer, &outlineRec);

	SDL_SetRenderDrawColor(renderer->sdlRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRectF(renderer->sdlRenderer, &newRect);
}

void UIElement::setVisible(const bool set) {
	visible = set;
}

bool UIElement::getVisible() const {
	return visible;
}

UILabel::UILabel(const SDL_FRect& rect, const SDL_Color& color, const float centered, const std::string& text) : UIElement(rect, color, centered) {
	this->text = text;
	this->centered = centered;
}

void UILabel::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	UIElement::draw(renderer, windowWidth, windowHeight);

	SDL_FRect newRect = getScaledPos(rect, windowWidth, windowHeight);

	FontManager::getInstance().drawText(renderer, text, FontManager::FontSize::MEDIUM, newRect.x, newRect.y, textColor, windowWidth, windowHeight);
}

UIButton::UIButton(const SDL_FRect& rect, const SDL_Color& color, const float centered, const std::string& text, void (*callback)(void*), void* context)
	: UIElement(rect, color, centered), text(text), context(context) {
	hover = false;
	click = false;

	clickColor = { 200, 200, 200, 255 };
	hoverColor = { 100, 100, 100, 255 };
	textColor = { 255,255,255,255 };

	this->callback = callback;
}

void UIButton::update(const SDL_FPoint& mousePos, const bool clicked, const unsigned int windowWidth, const unsigned int windowHeight) {
	// Scale mouse to actual window space
	SDL_FPoint scaledMousePos = {
		mousePos.x * windowWidth / vScreen::vWidth,
		mousePos.y * windowHeight / vScreen::vHeight
	};

	SDL_FRect scaledRect = getScaledPos(rect, windowWidth, windowHeight);

	if ((scaledMousePos.x >= scaledRect.x) && (scaledMousePos.x <= scaledRect.x + scaledRect.w) &&
		(scaledMousePos.y >= scaledRect.y) && (scaledMousePos.y <= scaledRect.y + scaledRect.h)) {

		hover = true;
		color = hoverColor;

		if (clicked) {
			color = clickColor;
			if (callback) {
				callback(context);
			}
		}
	}
	else {
		hover = false;
		color = defaultColor;
	}
}

void UIButton::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	UIElement::draw(renderer, windowWidth, windowHeight);

	SDL_FRect newRect = getScaledPos(rect, windowWidth, windowHeight);

	FontManager::getInstance().drawText(renderer, text, FontManager::FontSize::MEDIUM, newRect.x, newRect.y, textColor, windowWidth, windowHeight);
}