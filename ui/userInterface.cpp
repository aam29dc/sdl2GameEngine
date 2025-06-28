#include "userInterface.hpp"
#include "core/binds.hpp"

UIPage::UIPage(const std::string& name) : name(name) {}

UIPage::~UIPage() {
	for (UIElement* el : elements) {
		delete el;
	}
	elements.clear();
}

void UIPage::addElement(UIElement* element) {
	elements.push_back(element);
}

const std::vector<UIElement*>& UIPage::getElements() const {
	return elements;
}

std::vector<UIElement*>& UIPage::getElements() {
	return elements;
}

const std::string& UIPage::getName() const {
	return name;
}

UserInterface::UserInterface(Window* window, const std::string& name) : clicked(false), mousePos{0,0} {
	this->window = window;
	pages.insert({ name, UIPage(name) });
}

UserInterface::~UserInterface() {
	std::cout << "~UI called.\n";

	for (auto& [pageName, page] : pages) {
		std::cout << "Cleaning page: " << pageName << "\n";
		for (UIElement* el : page.getElements()) {
			std::cout << "del id: " << el->getID() << "\n";
			delete el;
		}
		page.getElements().clear();
	}

	pages.clear();
}

size_t UserInterface::addElement(UIElement* element) {
	auto& elements = pages[currentPage].getElements();
	elements.push_back(element);
	return element->getID();
}

void UserInterface::removeElement(const size_t& id) {
	auto& elements = pages.at(currentPage).getElements();
	for (auto it = elements.begin(); it != elements.end(); ++it) {
		if ((*it)->getID() == id) {
			delete* it;
			elements.erase(it);
			break;
		}
	}
}

UIElement* UserInterface::getElementByIndex(const size_t index) const {
	const auto& elements = pages.at(currentPage).getElements();
	if (index < elements.size()) {
		return elements[index];
	}
	return nullptr;
}

UIElement* UserInterface::getElement(const size_t& id) const {
	const auto& elements = pages.at(currentPage).getElements();
	for (UIElement* el : elements) {
		if (el->getID() == id) {
			return el;
		}
	}
	return nullptr;
}

UIElement* UserInterface::getElementByName(const std::string& name) const {
	const auto& elements = pages.at(currentPage).getElements();
	for (UIElement* el : elements) {
		if (el->getName() == name) {
			return el;
		}
	}
	return nullptr;
}

size_t UserInterface::getSize() const {
	return pages.at(currentPage).getElements().size();
}

void UserInterface::removeElement(const std::string& name) {
	auto& elements = pages[currentPage].getElements();
	for (auto it = elements.begin(); it != elements.end(); ++it) {
		if ((*it)->getName() == name) {
			delete* it;
			elements.erase(it);
			break;
		}
	}
}

void UserInterface::handleEvents() {
	clicked = Input::getInputHandler()->isMouseKeyReleased(MouseButtons::LEFT);
	mousePos = Input::getInputHandler()->getMousePosition();
}

void UserInterface::update() {
	auto& elements = pages[currentPage].getElements();
	for (UIElement* el : elements) {
		if (el->getVisible()) {
			el->update(mousePos, clicked, window->getWidth(), window->getHeight());
		}
	}
	clicked = false;
}

void UserInterface::render(Renderer* renderer) const {
	const auto& elements = pages.at(currentPage).getElements();
	for (UIElement* el : elements) {
		if (el->getVisible()) {
			el->draw(renderer, window->getWidth(), window->getHeight());
		}
	}
}

void UserInterface::switchToPage(const std::string& pageName) {
	if (pages.find(pageName) != pages.end()) {
		currentPage = pageName;
	}
	else {
		std::cerr << "Page not found: " << pageName << "\n";
	}
}