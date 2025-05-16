#include "window.hpp"
#include "managers/fontManager.hpp"
#include "enums/virtualScreen.hpp"

Window::Window() {
	windowHeight = 0;
	windowWidth = 0;
	mouseFocus = false;
	keyboardFocus = false;
	fullScreen = false;
	minimized = false;
	quit = false;
	_window = nullptr;
}

Window::~Window() {
	quit = true;
	SDL_DestroyWindow(_window);
}

bool Window::init(const int windowWidth, const int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	_window = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	return _window != nullptr;
}

void Window::handleWindowEvent(SDL_Event& e) {
	if (e.type == SDL_WINDOWEVENT) {
		bool updateCaption = false;
		int width = 0, height = 0;

		switch (e.window.event) {
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_GetWindowSizeInPixels(_window, &width, &height);
			this->windowWidth = width;
			this->windowHeight = height;
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			break;
		case SDL_WINDOWEVENT_ENTER:
			mouseFocus = true;
			updateCaption = true;
			break;
		case SDL_WINDOWEVENT_LEAVE:
			mouseFocus = false;
			updateCaption = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			keyboardFocus = true;
			updateCaption = true;
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			keyboardFocus = false;
			updateCaption = true;
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			minimized = true;
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			minimized = false;
			break;
		case SDL_WINDOWEVENT_RESTORED:
			minimized = false;
			break;
		case SDL_WINDOWEVENT_CLOSE:
			quit = true;
			break;
		}
	}
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
		if (fullScreen) {
			SDL_SetWindowFullscreen(_window, 0);
			fullScreen = false;
		}
		else {
			SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			fullScreen = true;
			minimized = false;
		}
	}
}

const unsigned int Window::getWidth() const {
	return windowWidth;
}

const unsigned int Window::getHeight() const {
	return windowHeight;
}

const bool Window::getMouseFocus() const {
	return mouseFocus;
}

const bool Window::getKeyboardFocus() const {
	return keyboardFocus;
}

const bool Window::getFullscreen() const {
	return fullScreen;
}

const bool Window::getMinimized() const {
	return minimized;
}

const bool Window::getQuit() const {
	return quit;
}