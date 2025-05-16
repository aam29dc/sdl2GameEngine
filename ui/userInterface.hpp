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

	void addElement(UIElement* element);
	/*
		could store an id thats created instead of a name, make get id cmd of element, then removeElement(someelement.getid())...
	*/
	//void removeElement(const std::string& name);

	virtual void handleEvents();
	virtual void update();
	virtual void render(Renderer* renderer) const;
};