#include "binds.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <SDL_mouse.h>
#include <SDL_keyboard.h>

//default binds
Binds::Binds() {
	// Movement
	MoveBinds[toIndex(Movement::FORWARD)] = InputBind(SDL_SCANCODE_W);
	MoveBinds[toIndex(Movement::BACKWARD)] = InputBind(SDL_SCANCODE_S);
	MoveBinds[toIndex(Movement::LEFTWARD)] = InputBind(SDL_SCANCODE_A);
	MoveBinds[toIndex(Movement::RIGHTWARD)] = InputBind(SDL_SCANCODE_D);

	// Combat
	CombatBinds[toIndex(Combat::ATTACKONE)] = InputBind((Uint8)SDL_BUTTON_LEFT);

	// Interact
	InteractBinds[toIndex(Interact::USE)] = InputBind(SDL_SCANCODE_E);
	InteractBinds[toIndex(Interact::INVENTORY)] = InputBind(SDL_SCANCODE_I);
	InteractBinds[toIndex(Interact::PAUSE)] = InputBind(SDL_SCANCODE_ESCAPE);
	InteractBinds[toIndex(Interact::ITEM1)] = InputBind(SDL_SCANCODE_1);
	InteractBinds[toIndex(Interact::ITEM2)] = InputBind(SDL_SCANCODE_2);
	InteractBinds[toIndex(Interact::ITEM3)] = InputBind(SDL_SCANCODE_3);
	InteractBinds[toIndex(Interact::ITEM4)] = InputBind(SDL_SCANCODE_4);
	InteractBinds[toIndex(Interact::ITEM5)] = InputBind(SDL_SCANCODE_5);

	// Misc
	MiscBinds[toIndex(Misc::SAVE)] = InputBind(SDL_SCANCODE_H);
	MiscBinds[toIndex(Misc::ZOOMIN)] = InputBind(SDL_SCANCODE_EQUALS); // `=`
	MiscBinds[toIndex(Misc::ZOOMOUT)] = InputBind(SDL_SCANCODE_MINUS);  // `-`
	MiscBinds[toIndex(Misc::CONSOLE)] = InputBind(SDL_SCANCODE_GRAVE);
}

bool Binds::readBinds(const std::string& bindsFile) {
	std::ifstream file(bindsFile);

	if (!file.is_open()) return false;

	std::string line;
	std::string currentSection;

	auto strToScancode = [](const std::string& keyStr) -> SDL_Scancode {
		return SDL_GetScancodeFromName(keyStr.c_str());
	};

	auto strToMouseButton = [](const std::string& str) -> Uint8 {
		if (str == "MOUSE1") return SDL_BUTTON_LEFT;
		if (str == "MOUSE2") return SDL_BUTTON_RIGHT;
		if (str == "MOUSE3") return SDL_BUTTON_MIDDLE;
		return 0;
	};

	while (std::getline(file, line)) {
		line.erase(0, line.find_first_not_of(" \t\r\n"));
		if (line.empty() || line[0] == '#') continue;

		if (line[0] == '[') {
			currentSection = line.substr(1, line.find(']') - 1);
			continue;
		}

		auto eqPos = line.find('=');
		if (eqPos == std::string::npos) continue;

		std::string action = line.substr(0, eqPos);
		std::string value = line.substr(eqPos + 1);

		action.erase(0, action.find_first_not_of(" \t"));
		action.erase(action.find_last_not_of(" \t") + 1);
		value.erase(0, value.find_first_not_of(" \t"));
		value.erase(value.find_last_not_of(" \t") + 1);

		InputBind bind;
		if (value.rfind("MOUSE", 0) == 0) {
			bind = InputBind(strToMouseButton(value));
		}
		else {
			bind = InputBind(strToScancode(value));
		}

		if (currentSection == "Movement") {
			if (action == "Forward") MoveBinds[toIndex(Movement::FORWARD)] = bind;
			else if (action == "Backward") MoveBinds[toIndex(Movement::BACKWARD)] = bind;
			else if (action == "Leftward") MoveBinds[toIndex(Movement::LEFTWARD)] = bind;
			else if (action == "Rightward") MoveBinds[toIndex(Movement::RIGHTWARD)] = bind;
		}
		else if (currentSection == "Combat") {
			if (action == "Attack") CombatBinds[toIndex(Combat::ATTACKONE)] = bind;
		}
		else if (currentSection == "Interact") {
			if (action == "Use") InteractBinds[toIndex(Interact::USE)] = bind;
			else if (action == "Inventory") InteractBinds[toIndex(Interact::INVENTORY)] = bind;
			else if (action == "Pause") InteractBinds[toIndex(Interact::PAUSE)] = bind;
			else if (action == "Item1") InteractBinds[toIndex(Interact::ITEM1)] = bind;
			else if (action == "Item2") InteractBinds[toIndex(Interact::ITEM2)] = bind;
			else if (action == "Item3") InteractBinds[toIndex(Interact::ITEM3)] = bind;
			else if (action == "Item4") InteractBinds[toIndex(Interact::ITEM4)] = bind;
			else if (action == "Item5") InteractBinds[toIndex(Interact::ITEM5)] = bind;
		}
		else if (currentSection == "Misc") {
			if (action == "Save") MiscBinds[toIndex(Misc::SAVE)] = bind;
			else if (action == "ZoomIn") MiscBinds[toIndex(Misc::ZOOMIN)] = bind;
			else if (action == "ZoomOut") MiscBinds[toIndex(Misc::ZOOMOUT)] = bind;
			else if (action == "Console") MiscBinds[toIndex(Misc::CONSOLE)] = bind;
		}
	}

	return true;
}

const InputBind& Binds::getMoveBind(Movement m) const {
	return MoveBinds[toIndex(m)];
}

void Binds::setMoveBind(Movement m, const InputBind& bind) {
	MoveBinds[toIndex(m)] = bind;
}

const InputBind& Binds::getCombatBind(Combat c) const {
	return CombatBinds[toIndex(c)];
}

void Binds::setCombatBind(Combat c, const InputBind& bind) {
	CombatBinds[toIndex(c)] = bind;
}

const InputBind& Binds::getInteractBind(Interact i) const {
	return InteractBinds[toIndex(i)];
}

void Binds::setInteractBind(Interact i, const InputBind& bind) {
	InteractBinds[toIndex(i)] = bind;
}

const InputBind& Binds::getMiscBind(Misc m) const {
	return MiscBinds[toIndex(m)];
}

void Binds::setMiscBind(Misc m, const InputBind& bind) {
	MiscBinds[toIndex(m)] = bind;
}