#pragma once

#include "ui/uiElement.hpp"
#include "core/input.hpp"
#include "core/window.hpp"

#include <vector>

class UserInterface {
private:
	std::vector<UIElement*> UIElements;
	bool clicked;
	SDL_FPoint mousePos;
	Window* window;
public:
	UserInterface(Window *window);
	virtual ~UserInterface();

	virtual size_t addElement(UIElement* element);
	virtual void removeElement(const size_t& id) const;
	virtual UIElement* getElement(const size_t& id) const;
	virtual UIElement* getElementByIndex(const size_t index) const;
	virtual UIElement* getElementByName(const std::string& name) const;
	virtual size_t getSize() const;

	virtual void handleEvents();
	virtual void update();
	virtual void render(Renderer* renderer) const;
};