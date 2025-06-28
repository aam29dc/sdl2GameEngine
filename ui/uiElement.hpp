#pragma once
#include "core/renderer.hpp"
#include "managers/fontManager.hpp"
#include "enums/virtualScreen.hpp"
#include "enums/color.hpp"
#include <vector>
#include <iostream>
#include <utility>

class UIElement {
protected:
	SDL_FRect rect;
	SDL_Color color;
	SDL_Color defaultColor;
	SDL_Color outlineColor;
	SDL_Color textColor;
	bool centered;
	bool visible;
	size_t id;
	std::string name;
	inline static size_t instanceCount = 0;
public:
	UIElement(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const bool centered = false,
		const std::string& name = "");

	virtual ~UIElement() { 
		--instanceCount;
		std::cout << "Deconstructor called.\n";
		std::cout << "iC:" << instanceCount << "\n";
	}

	SDL_FRect getScaledPos(const SDL_FRect& rect, const unsigned int windowWidth, const unsigned int windowHeight) const;

	virtual void update(const SDL_FPoint& mousePos = { 0,0 }, const bool clicked = false, const unsigned int windowWidth = 0, const unsigned int windowHeight = 0) {};
	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const;

	void toggleVisible();
	bool getVisible() const;

	size_t getID() const;
	void setID(const size_t& val);

	const std::string& getName() const;
};

class UILabel : public UIElement {
protected:
	std::string text;
public:
	UILabel(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const std::string& text = "",
		const bool centered = false);
	virtual ~UILabel() {}

	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const;
};

class UITextBox : public UIElement {
protected:
	std::vector<std::string> lines;
	int index;
public:
	UITextBox(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const bool centered = false, const std::string& name = "TextBox");
	virtual ~UITextBox() {}

	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const;
	virtual void addLine(const std::string& text);
	
	virtual size_t getSize() const;
	virtual void changeIndex(const int val);
};

class UIConsole : public UITextBox {
protected:
	size_t cur;
	std::vector<std::string> input;
	std::string currentInputLine;
public:
	UIConsole(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const bool centered = false, const std::string& name = "Console");
	virtual ~UIConsole() {}

	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const;

	virtual std::string getInput(const size_t index);
	virtual void delChar();
	virtual void endLine();
	virtual void replaceLine(const std::string& text);
	virtual void addChar(const std::string& ch);
	virtual size_t getCur() const;
	virtual void handleInput();
	virtual void print(const std::string& text);
};

class UIButton : public UIElement {
protected:
	bool hover;
	bool click;

	SDL_Color clickColor;
	SDL_Color hoverColor;
	SDL_Color textColor;
	std::string text;

	void (*callback)(void*);
	void* context;
public:
	UIButton(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const std::string& text = "",
		void (*callback)(void*) = nullptr,
		void* context = nullptr,
		const bool centered = false);

	virtual ~UIButton(){}

	virtual void update(const SDL_FPoint& mousePos, const bool clicked, const unsigned int windowWidth, const unsigned int windowHeight) override;
	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const override;
};

class UITexture : public UIButton {
protected:
	size_t textureID;
	SDL_Rect src;
public:
	UITexture(const SDL_FRect& rect = { 0 },
		const size_t& id = 0,
		const SDL_Rect& src = {0},
		const SDL_Color& color = Color::Shade,
		void (*callback)(void*) = nullptr,
		void* context = nullptr,
		const bool centered = false);
	virtual ~UITexture() {}

	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const override;

	void setSrc(const SDL_Rect& src);
};

template<typename C, typename T>
class UIText : public UIElement {
protected:
	const C* instance;
	std::vector<std::string> strs;
	std::vector<T(C::*)() const> values;
	std::vector<T(*)(T)> evaluators;
public:
	UIText(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const C* instance = nullptr,
		const std::vector<std::string>& strings = {},
		const std::vector<T(C::*)() const>& valueFuncs = {},
		const std::vector<T(*)(T)>& evalFuncs = {},
		const bool centered = false)
		: UIElement(rect, color, centered), instance(instance), strs(strings), values(valueFuncs), evaluators(evalFuncs) {
	}

	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const override {
		UIElement::draw(renderer, windowWidth, windowHeight);

		SDL_FRect newRect = getScaledPos(rect, windowWidth, windowHeight);

		std::string result;
		size_t maxSize = std::max({ strs.size(), values.size(), evaluators.size() });

		for (size_t i = 0; i < maxSize; ++i) {
			if (i < strs.size()) result += strs[i];
			if (i < values.size() && instance) {
				T val = (instance->*values[i])();
				if (i < evaluators.size() && evaluators[i]) {
					val = evaluators[i](val);
				}
				result += std::to_string((int)val);
			}
		}

		FontManager::getInstance().drawText(renderer,
			result,
			FontManager::FontSize::MEDIUM,
			newRect.x, newRect.y, textColor,
			windowWidth, windowHeight
		);
	}
};

template<typename C, typename T>
class UIResource : public UIElement {
private:
	const C* instance;
	T (C::*resource)() const;
	T (C::*maxResource)() const;
public:
	UIResource(const SDL_FRect& rect = { 0 },
		const SDL_Color& color = Color::Shade,
		const C* instance = nullptr,
		T (C::* resource)() const = nullptr,
		T (C::* maxResource)() const = nullptr,
		const bool centered = false) : UIElement(rect, color, centered) {
			this->instance = instance;
			this->resource = resource;
			this->maxResource = maxResource;
	}

	virtual void draw(Renderer* renderer, const unsigned int windowWidth, const unsigned int windowHeight) const override {
		SDL_FRect newRect = getScaledPos(rect, windowWidth, windowHeight);

		SDL_FRect outlineRec = rect;
		outlineRec.x--;
		outlineRec.y--;
		outlineRec.w += 2;
		outlineRec.h += 2;
		outlineRec = renderer->scale(outlineRec);

		SDL_SetRenderDrawColor(renderer->sdlRenderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
		SDL_RenderDrawRectF(renderer->sdlRenderer, &outlineRec);

		SDL_SetRenderDrawColor(renderer->sdlRenderer, color.r, color.g, color.b, color.a);

		SDL_FRect resourceRec = renderer->scale(rect);

		if ((instance->*resource)() <= 0) resourceRec.w = 0;
		else resourceRec.w *= (instance->*resource)() / float((instance->*maxResource)());

		SDL_RenderFillRectF(renderer->sdlRenderer, &resourceRec);

		FontManager::getInstance().drawText(renderer,
			std::to_string((int)(instance->*resource)()),
			FontManager::FontSize::MEDIUM,
			newRect.x, newRect.y,
			textColor, windowWidth, windowHeight
		);
	}
};