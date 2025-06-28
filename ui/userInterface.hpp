#pragma once

#include "ui/uiElement.hpp"
#include "core/input.hpp"
#include "core/window.hpp"
#include <unordered_map>
#include <vector>

class UIPage {
private:
	std::string name;
	std::vector<UIElement*> elements;
public:
	UIPage(const std::string& name = "Unnamed");
	~UIPage();
	void addElement(UIElement* element);
	const std::vector<UIElement*>& getElements() const;
	std::vector<UIElement*>& getElements();
	const std::string& getName() const;
};

class UserInterface {
private:
	std::unordered_map<std::string, UIPage> pages;
	std::string currentPage;

	bool clicked;
	SDL_FPoint mousePos;
	Window* window;
public:
	UserInterface(Window* window = nullptr, const std::string& name = "Unnamed");
	virtual ~UserInterface();

	void switchToPage(const std::string& pageName);

	virtual size_t addElement(UIElement* element);
	virtual void removeElement(const size_t& id);
	virtual void removeElement(const std::string& name);
	virtual UIElement* getElement(const size_t& id) const;
	virtual UIElement* getElementByIndex(const size_t index) const;
	virtual UIElement* getElementByName(const std::string& name) const;
	virtual size_t getSize() const;

	virtual void handleEvents();
	virtual void update();
	virtual void render(Renderer* renderer) const;
};