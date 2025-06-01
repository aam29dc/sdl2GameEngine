#include "userInterface.hpp"

UserInterface::UserInterface(Window* window = nullptr) : clicked(false), mousePos{ 0,0 } {
	this->window = window;

	addElement(new UITextBox({ 0, 0, 1440, 450 }, Color::Shade, false, "Console"));
	dynamic_cast<UITextBox*>(getElementByName("Console"))->toggleVisible();
}

UserInterface::~UserInterface() {
	std::cout << "~UI called.\n";
	for (auto it = UIElements.cbegin(); it != UIElements.cend(); ++it) {
		std::cout << "del id: " << (*it)->getID() << "\n";
		delete *it;
	}

	UIElements.clear();
}

size_t UserInterface::addElement(UIElement* element) {
	UIElements.push_back(element);
	return element->getID();
}

void UserInterface::removeElement(const size_t& id) const {
	if (UIElements.at(id)) {
		delete UIElements.at(id);
		std::cout << "del id: " << "\n";
	}
}

UIElement* UserInterface::getElement(const size_t& id) const {
	for (auto* el : UIElements) {
		if (el->getID() == id) {
			std::cout << "return'd " << id << "\n";
			return el;
		}
	}
	return nullptr;
}

UIElement* UserInterface::getElementByIndex(const size_t index) const {
	if (index < UIElements.size())
		return UIElements[index];
	return nullptr;
}

UIElement* UserInterface::getElementByName(const std::string& name) const {
	for (auto* el : UIElements) {
		if (el->getName() == name) {
			return el;
		}
	}
	return nullptr;
}

size_t UserInterface::getSize() const {
	return UIElements.size();
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