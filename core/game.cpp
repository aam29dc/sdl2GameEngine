#include "game.hpp"

#include "core/window.hpp"
#include "core/renderer.hpp"
#include "core/input.hpp"
#include "managers/textureManager.hpp"
#include "managers/soundManager.hpp"
#include "managers/fontManager.hpp"
#include "states/gameStateMachine.hpp"
#include "states/menuState.hpp"
#include "states/playState.hpp"
#include "managers/cvarManager.hpp"
#include "core/time.hpp"
#include "ui/uiElement.hpp"
#include "core/binds.hpp"

#ifdef DEBUG
	#include <iostream>
#endif

Game::Game() {
	window = nullptr;
	renderer = nullptr;
	soundManager = nullptr;
	gameStateMachine = nullptr;
	defaultTextureID = 0;
	binds = nullptr;
	console = nullptr;
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

		//load default texture, with textureID = 0, this is for all failed loaded textures
		//const std::string path = "assets/textures/default.png";
		//TextureManager::getInstance()->load(renderer, defaultTextureID, path);

		//cvars
		registerCvars();

		//binds
		binds = new Binds();
		binds->readBinds("assets/binds.ini");

		//console
		console = new UIConsole({ 0, 0, 1440, 450 });
		console->toggleVisible();

		console->print("Welcome to Neiths adventure.");

		//gamestatemachine
		gameStateMachine = new GameStateMachine(window, renderer, soundManager);
		gameStateMachine->queuePush(new MenuState(window, gameStateMachine, console, binds));

		return true;
	}

	return false;
}

void Game::handleEvents() const {
	Input::getInputHandler()->update(window);

	if (Input::getInputHandler()->isKeyReleased(binds->getMiscBind(Misc::CONSOLE).key)) {
		console->toggleVisible();
	}

	if (console->getVisible()) {
		console->handleInput();
	}

	gameStateMachine->handleEvents();
}

bool Game::update() const {
	bool empty = gameStateMachine->update();

	Camera* camera = nullptr;

	if (auto* state = dynamic_cast<PlayState*>(gameStateMachine->getCurrentState())) {
		camera = state->getCamera();
	}

	if(camera) soundManager->update(camera);

	return empty;
}

void Game::render() const {
	SDL_SetRenderDrawColor(renderer->sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer->sdlRenderer);
	gameStateMachine->render(renderer);
	console->draw(renderer, window->getWidth(), window->getHeight());
	SDL_RenderPresent(renderer->sdlRenderer);
}

void Game::registerCvars() {
	Cvar<float>* c = new Cvar<float>("timescale",
		[](const float& t) { Time::getInstance()->setTimeScale(t); },
		[]() { return Time::getInstance()->getTimeScale(); });

	CvarManager::getInstance().registerCvar(c);

	Cvar<bool>* quitCvar = new Cvar<bool>(
		"quit",
		[&](const bool& v) { getWindow()->setQuit(v); },
		[&]() { return getWindow()->getQuit(); }
	);

	CvarManager::getInstance().registerCvar(quitCvar);

	Cvar<float>* dmgMultCvar = new Cvar<float>(
		"dmgtaken",
		[&](const float& v) {
			auto player = getCurrentPlayer();
			if (player) player->setDamageTakenMultiplier(v);
		},
		[&]() -> float {
			auto player = getCurrentPlayer();
			return player ? player->getDamageTakenMultiplier() : 1.0f;
		}
	);
	CvarManager::getInstance().registerCvar(dmgMultCvar);
}

Window* Game::getWindow() const {
	return window;
}

Player* Game::getCurrentPlayer() {
	GameState* current = gameStateMachine->getCurrentState();
	PlayState* playState = dynamic_cast<PlayState*>(current);
	if (playState) return playState->getPlayer();
	return nullptr;
}