#include "input.hpp"
#include "core/window.hpp"
#include "enums/virtualScreen.hpp"

Input* Input::_input = nullptr;

Input::Input() {
	for (int i = 0; i < 3; i++)	{
		_mouseButtonStates.push_back(false);
	}
	_prevMouseButtonStates = _mouseButtonStates;
	_mPos.x = 0;
	_mPos.y = 0;
	_keyStates = nullptr;
	for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
		_prevKeyStates[i] = 0;
	}
	_numkeys = 0;
}

Input* Input::getInputHandler() {
	if (_input == nullptr) _input = new Input();
	return _input;
}

bool Input::update(Window* const window) {
	SDL_Event event;

	_updatePrevKeyStates();		//without this then when user presses a key, the key will show up as being pressed multiple times

	while (SDL_PollEvent(&event)) {
		switch (event.type)	{
		case SDL_MOUSEBUTTONDOWN:
			_onMouseButtonDown(event);
			break;
		case SDL_MOUSEBUTTONUP:
			_onMouseButtonUp(event);
			break;
		case SDL_MOUSEMOTION:
			_onMouseMotion(event, window->getHeight(), window->getWidth());
			break;
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			_keyStates = SDL_GetKeyboardState(&_numkeys);
			break;
		case SDL_WINDOWEVENT:
			window->handleWindowEvent(event);
			if (window->getQuit() == true) {
				return true;
			}
			break;
		case SDL_QUIT:
			return true;
			break;
		default:
			break;
		}
	}
	return false;
}

void Input::_onMouseButtonDown(const SDL_Event& event) {
	if (event.button.button == SDL_BUTTON_LEFT)	{
		_mouseButtonStates[LEFT] = true;
	}
	if (event.button.button == SDL_BUTTON_MIDDLE) {
		_mouseButtonStates[MIDDLE] = true;
	}
	if (event.button.button == SDL_BUTTON_RIGHT) {
		_mouseButtonStates[RIGHT] = true;
	}
}

void Input::_onMouseButtonUp(const SDL_Event& event)
{
	if (event.button.button == SDL_BUTTON_LEFT)	{
		_mouseButtonStates[LEFT] = false;
	}
	if (event.button.button == SDL_BUTTON_MIDDLE) {
		_mouseButtonStates[MIDDLE] = false;
	}
	if (event.button.button == SDL_BUTTON_RIGHT) {
		_mouseButtonStates[RIGHT] = false;
	}
}

void Input::_onMouseMotion(const SDL_Event& event, const unsigned int windowHeight, const unsigned int windowWidth) {
	_mPos.x = (float)event.motion.x * vScreen::vWidth / windowWidth;
	_mPos.y = (float)event.motion.y * vScreen::vHeight / windowHeight;
}

bool Input::isKeyDown(const SDL_Scancode& key) const {
	if (_keyStates != 0 && _keyStates[key] == 1) {
		return true;
	}
	return false;
}

SDL_Scancode Input::getFirstKeyDown() const {
	if (_keyStates != nullptr) {
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
			if (_keyStates[i]) {
				return static_cast<SDL_Scancode>(i);
			}
		}
	}
	return SDL_SCANCODE_UNKNOWN;
}

SDL_Scancode Input::getFirstKeyReleased() const {
	if (_keyStates != nullptr && _prevKeyStates != nullptr) {
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
			if (_prevKeyStates[i] && !_keyStates[i]) {
				return static_cast<SDL_Scancode>(i);
			}
		}
	}
	return SDL_SCANCODE_UNKNOWN;
}

bool Input::isAnyKeyReleased() const {
	if (_keyStates != nullptr && _prevKeyStates != nullptr) {
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
			if (_prevKeyStates[i] && !_keyStates[i]) {
				return true;
			}
		}
	}
	return false;
}

bool Input::isAnyKeyDown() const {
	if (_keyStates != nullptr) {
		for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
			if (_keyStates[i]) {
				return true;
			}
		}
	}
	return false;
}

/* registers a 'release' when prev key state is 0, then current key state is 1 (rather than the other way around, as to register when the key is hit)
*/
bool Input::isKeyReleased(const SDL_Scancode& key) const {
	if ((_keyStates != 0) && (_prevKeyStates != 0) && (_prevKeyStates[key] == 0) && (_keyStates[key] == 1)) {
		return true;
	}
	else return false;
}

bool Input::isMouseKeyReleased(const int buttonNumber) const {
	if(_prevMouseButtonStates[buttonNumber] == 1 && _mouseButtonStates[buttonNumber] == 0) return true;
	return false;
}

const bool Input::getMouseButtonState(const int buttonNumber) const {
	return _mouseButtonStates[buttonNumber];
}

const SDL_FPoint& Input::getMousePosition() const {
	return _mPos;
}

void Input::_updatePrevKeyStates() {
	if (_keyStates != 0) {
		memcpy(_prevKeyStates, _keyStates, _numkeys);
	}

	_prevMouseButtonStates = _mouseButtonStates;
}