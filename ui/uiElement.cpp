#include "uiElement.hpp"
#include "core/renderer.hpp"
#include "managers/textureManager.hpp"

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

UIElement::UIElement(const SDL_FRect& rect, const SDL_Color& color, const bool centered, const std::string& name) {
	this->rect = rect;
	this->color = color;
	this->defaultColor = color;
	this->outlineColor = Color::Black;
	this->textColor = Color::Red;
	this->centered = centered;
	this->visible = true;
	this->id = instanceCount;
	this->name = name;
	instanceCount++;
	std::cout << "created: " << id << "\n";
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

void UIElement::toggleVisible() {
	visible = !visible;
}

bool UIElement::getVisible() const {
	return visible;
}

size_t UIElement::getID() const {
	return id;
}

void UIElement::setID(const size_t& val) {
	id = val;
}

const std::string& UIElement::getName() const {
	return name;
}

UILabel::UILabel(const SDL_FRect& rect, const SDL_Color& color, const std::string& text, const bool centered) : UIElement(rect, color, centered) {
	this->text = text;
	this->centered = centered;
	this->id = instanceCount;
}

void UILabel::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	UIElement::draw(renderer, windowWidth, windowHeight);
	SDL_FRect newRect = getScaledPos(rect, windowWidth, windowHeight);
	FontManager::getInstance().drawText(renderer, text, FontManager::FontSize::MEDIUM, newRect.x, newRect.y, textColor, windowWidth, windowHeight);
}

UITextBox::UITextBox(const SDL_FRect& rect, const SDL_Color& color, const bool centered, const std::string& name) : UIElement(rect, color, centered) {
	this->textColor = Color::White;
	this->name = name;
	this->index = 0;
}

/*	BUG: When window is scaled down the text goes outside the rect. */
void UITextBox::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	if (!visible) return;
	UIElement::draw(renderer, windowWidth, windowHeight);

	SDL_FRect lineRect = { rect.x, rect.y + rect.h - 20.0f, rect.w, 20.0f };
	lineRect = getScaledPos(lineRect, windowWidth, windowHeight);

	unsigned int i = 0;

	for (int h = 0; h < rect.h; h+=((int)rect.h/5)) {
		if (i >= lines.size()) break;
		
		FontManager::getInstance().drawText(renderer, lines.at(lines.size() - 1 - i), FontManager::FontSize::MEDIUM, lineRect.x, lineRect.y - h, textColor, windowWidth, windowHeight);
		i++;
	}
}

void UITextBox::endLine() {
	lines.emplace_back("");
}

void UITextBox::addLine(const std::string& text) {
	lines.emplace_back(text);
}

void UITextBox::addChar(const std::string& ch) {
	if (lines.empty()) lines.push_back(ch);
	else lines.back().append(ch);
}

void UITextBox::changeIndex(const int val) {
	this->index += val;
}

UIButton::UIButton(const SDL_FRect& rect, const SDL_Color& color, const std::string& text, void (*callback)(void*), void* context, const bool centered)
	: UIElement(rect, color, centered), text(text), context(context) {
	hover = false;
	click = false;

	clickColor = Color::Light;
	hoverColor = Color::Dark;
	textColor = Color::White;

	this->callback = callback;
	this->id = instanceCount;
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

UITexture::UITexture(const SDL_FRect& rect, const size_t& id, const SDL_Rect& src, const SDL_Color& color, void (*callback)(void*), void* context, const bool centered) : UIButton(rect, color, "", callback, context, centered) {
	this->textureID = id;
	this->src = src;
	this->id = instanceCount;
}

void UITexture::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	UIButton::draw(renderer, windowWidth, windowHeight);
	renderer->draw(textureID, src, rect);
}

void UITexture::setSrc(const SDL_Rect& src) {
	this->src = src;
}