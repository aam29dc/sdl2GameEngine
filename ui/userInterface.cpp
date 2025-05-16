#include "userInterface.hpp"

UserInterface::UserInterface(Window* window = nullptr) : clicked(false), mousePos{ 0,0 } {
	this->window = window;
}

UserInterface::~UserInterface() {
	for (auto it = UIElements.cbegin(); it != UIElements.cend(); ++it) {
		delete *it;
	}

	UIElements.clear();
}

void UserInterface::addElement(UIElement* element) {
	UIElements.push_back(element);
}

/*void UserInterface::removeElement(const std::string& name) {
	for (int i = 0; i < UIElements.size(); ++i) {
		if (UIElements[i]->getName() == name) {
			delete UIElements[i];
			UIElements.erase(UIElements.begin() + i);
			break;
		}
	}
}*/

void UserInterface::handleEvents() {
	clicked = Input::getInputHandler()->isMouseKeyReleased(MouseButtons::LEFT);
	mousePos = Input::getInputHandler()->getMousePosition();
}

void UserInterface::update() {
	for (auto it = UIElements.cbegin(); it != UIElements.cend(); ++it) {
		if ((*it)->getVisible()) {
			(*it)->update(mousePos, clicked, window->getWidth(), window->getHeight());
		}
	}
	clicked = false;
}

void UserInterface::render(Renderer *renderer) const {
	for (auto it = UIElements.cbegin(); it != UIElements.cend(); ++it) {
		if ((*it)->getVisible()) {
			(*it)->draw(renderer, window->getWidth(), window->getHeight());
		}
	}
}