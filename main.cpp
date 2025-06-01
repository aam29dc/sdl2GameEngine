#include "game.hpp"
#include "core/window.hpp"
#include "core/time.hpp"

#include <iostream>

#undef main
int main() {
	int windowWidth = 1440;
	int windowHeight = 900;

	Game game;

	if (!game.init(windowWidth, windowHeight)) {
		return 1;
	}

	Time::getInstance()->setStartTime();

	bool quit = false;

	while (!game.getWindow()->getQuit() && !quit) {
		Time::getInstance()->tick();

		game.handleEvents();
		quit = game.update();
		game.render();
	}

	return 0;
}