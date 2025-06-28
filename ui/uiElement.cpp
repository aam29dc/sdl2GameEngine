#include "uiElement.hpp"
#include "core/renderer.hpp"
#include "managers/textureManager.hpp"
#include <algorithm>

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

	int next = (int)rect.h / 20;

	for (int h = 0; h < rect.h; h+=((int)rect.h/next)) {
		if (i >= lines.size()) break;
		
		FontManager::getInstance().drawText(renderer, lines.at(lines.size() - 1 - i), FontManager::FontSize::MEDIUM, lineRect.x, lineRect.y - h, textColor, windowWidth, windowHeight);
		i++;
	}
}

size_t UITextBox::getSize() const {
	return lines.size();
}

void UITextBox::changeIndex(const int val) {
	this->index += val;
}

void UITextBox::addLine(const std::string& text) {
	lines.emplace_back(text);
}

UIConsole::UIConsole(const SDL_FRect& rect,	const SDL_Color& color,	const bool centered, const std::string& name) : UITextBox(rect, color, centered, name) {
	this->cur = input.size();
}

void UIConsole::draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const {
	if (!visible) return;
	UIElement::draw(renderer, windowWidth, windowHeight);

	// Reserve 1 line at the bottom for input
	const float lineHeight = 20.0f;
	const int maxVisibleLines = (int)(rect.h / lineHeight) - 1; // One less to reserve for input
	SDL_FRect scaledRect = getScaledPos(rect, windowWidth, windowHeight);

	// Draw history lines
	for (int i = 0; i < maxVisibleLines; ++i) {
		size_t lineIndex = (lines.size() >= maxVisibleLines)
			? lines.size() - maxVisibleLines + i
			: i;

		if (lineIndex >= lines.size()) break;

		float y = scaledRect.y + lineHeight * i;
		FontManager::getInstance().drawText(renderer, lines[lineIndex],
			FontManager::FontSize::MEDIUM,
			scaledRect.x,
			y,
			textColor,
			windowWidth,
			windowHeight);
	}

	// Draw current input line at the bottom
	float inputY = scaledRect.y + scaledRect.h - lineHeight;
	FontManager::getInstance().drawText(renderer, currentInputLine,
		FontManager::FontSize::MEDIUM,
		scaledRect.x,
		inputY,
		textColor,
		windowWidth,
		windowHeight);
}

void UIConsole::delChar() {
	if (!currentInputLine.empty()) {
		currentInputLine.pop_back();
	}
}

void UIConsole::endLine() {
	input.push_back(currentInputLine);   // Save to input history
	lines.push_back(currentInputLine);   // Display it as output
	currentInputLine.clear();            // Reset for next line
	cur = input.size();                  // Reset history cursor
}

void UIConsole::replaceLine(const std::string& text) {
	currentInputLine = text;
}

void UIConsole::addChar(const std::string& ch) {
	currentInputLine.append(ch);
}

size_t UIConsole::getCur() const {
	return cur;
}

std::string UIConsole::getInput(const size_t index) {
	if (input.empty()) return "";

	// Clamp index to valid range
	cur = std::clamp(index, size_t(0), input.size() - 1);
	return input[cur];
}

void UIConsole::print(const std::string& text) {
	lines.emplace_back(text);
	endLine();
}

#include "core/input.hpp"
#include "managers/cvarManager.hpp"
#include <sstream>

void UIConsole::handleInput() {
	auto inputHandler = Input::getInputHandler();
	SDL_Scancode key = inputHandler->getFirstKeyReleased();

	if (key != SDL_SCANCODE_UNKNOWN) {
		if (key == SDL_SCANCODE_RETURN) {
			endLine();

			size_t historyIndex = (getSize() >= 2) ? (getSize() - 2) : 0;
			std::string line = getInput(historyIndex);
			std::istringstream input(line);
			std::string cmd;
			input >> cmd;

			std::cout << line << "\n";

			cmd.erase(0, cmd.find_first_not_of(" \t\r\n"));
			cmd.erase(cmd.find_last_not_of(" \t\r\n") + 1);

			BaseCvar* cvar = CvarManager::getInstance().getCvar(cmd);

			if (cvar) {
				cvar->setFromString(cvar->extractCvarValue(line));
			}
			else std::cout << "Unknown cvar/cmd.\n";
		}
		else if (key == SDL_SCANCODE_BACKSPACE || key == SDL_SCANCODE_DELETE) {
			delChar();
		}
		else if (key >= SDL_SCANCODE_A && key <= SDL_SCANCODE_Z) {
			bool shift = inputHandler->isKeyDown(SDL_SCANCODE_LSHIFT) || inputHandler->isKeyDown(SDL_SCANCODE_RSHIFT);
			char c = (shift) ? ('A' + (key - SDL_SCANCODE_A)) : ('a' + (key - SDL_SCANCODE_A));
			addChar(std::string(1, c));
		}
		else if (key >= SDL_SCANCODE_1 && key <= SDL_SCANCODE_0) {
			bool shift = inputHandler->isKeyDown(SDL_SCANCODE_LSHIFT) || inputHandler->isKeyDown(SDL_SCANCODE_RSHIFT);

			const char normalNums[] = "1234567890";
			const char shiftedNums[] = "!@#$%^&*()";

			int index = (key == SDL_SCANCODE_0) ? 9 : (key - SDL_SCANCODE_1);
			if (index >= 0 && index < 10) {
				char outputChar = shift ? shiftedNums[index] : normalNums[index];
				addChar(std::string(1, outputChar));
			}
		}
		else if (key == SDL_SCANCODE_UP) {
			if (cur > 0) cur--;
			replaceLine(getInput(cur));
		}
		else if (key == SDL_SCANCODE_DOWN) {
			if (cur < input.size() - 1) {
				cur++;
				replaceLine(getInput(cur));
			}
			else {
				cur = input.size(); // move past last item
				replaceLine("");
			}
		}
		else if (key == SDL_SCANCODE_SPACE) {
			addChar(" ");
		}
		else if (key == SDL_SCANCODE_MINUS) {
			addChar("-");
		}
		else if (key == SDL_SCANCODE_PERIOD) {
			addChar(".");
		}
		else if (key == SDL_SCANCODE_EQUALS) {
			addChar("=");
		}
	}
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