#include "game.hpp"

#include "core/window.hpp"
#include "core/renderer.hpp"
#include "core/input.hpp"
#include "core/camera.hpp"
#include "managers/textureManager.hpp"
#include "managers/soundManager.hpp"
#include "managers/fontManager.hpp"
#include "states/gameStateMachine.hpp"
#include "states/menuState.hpp"
#include "states/playState.hpp"

#ifdef DEBUG
	#include <iostream>
#endif

Game::Game() {
	window = nullptr;
	renderer = nullptr;
	soundManager = nullptr;
	gameStateMachine = nullptr;
	defaultTextureID = 0;
}

Game::~Game() {
	if (renderer) delete renderer;
	renderer = nullptr;

	if (gameStateMachine) delete gameStateMachine;
	gameStateMachine = nullptr;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Game::init(const int windowWidth, const int windowHeight) {
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
		window = new Window();
		if (!window->init(windowWidth, windowHeight)) return false;

		renderer = new Renderer();
		if (!renderer->init(window, TextureManager::getInstance())) return false;

		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			#ifdef DEBUG
				std::cout << "Warning: Linear texture filtering not enabled!" << "\n";
			#endif
			return false;
		}

		if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
			#ifdef DEBUG
				std::cout << "SDL_image could not initialize! SDL_image Error :" << IMG_GetError() << "\n";
			#endif
			return false;
		}

		if (!FontManager::getInstance().init()) {
			#ifdef DEBUG
				std::cout << "Init font error SDL_ttf: " << TTF_GetError() << "\n";
			#endif
			return false;
		}

		SDL_SetRenderDrawBlendMode(renderer->sdlRenderer, SDL_BLENDMODE_BLEND);

		soundManager = new SoundManager();
		if (!soundManager->init()) return false;

		gameStateMachine = new GameStateMachine(window, renderer, soundManager);
		gameStateMachine->queuePush(new MenuState(window, gameStateMachine));

		//load default texture, with textureID = 0, this is for all failed loaded textures
		//const std::string path = "assets/textures/default.png";
		//TextureManager::getInstance()->load(renderer, defaultTextureID, path);

		return true;
	}
	return false;
}

void Game::handleEvents() const {
	Input::getInputHandler()->update(window);
	gameStateMachine->handleEvents();
}

bool Game::update() const {
	bool empty = gameStateMachine->update();

	Camera* camera = nullptr;

	if (auto* state = dynamic_cast<PlayState*>(gameStateMachine->getCurrentState())) {
		camera = state->getCamera();
	}

	soundManager->update(camera);

	return empty;
}

void Game::render() const {
	SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer->sdlRenderer);
	gameStateMachine->render(renderer);
	SDL_RenderPresent(renderer->sdlRenderer);
}

Window* Game::getWindow() const {
	return window;
}