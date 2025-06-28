#pragma once

#include <SDL_video.h>
#include <SDL_events.h>

class Window {
private:
	unsigned int windowHeight;
	unsigned int windowWidth;

	bool mouseFocus;
	bool keyboardFocus;
	bool fullScreen;
	bool minimized;
	bool quit;
public:
	SDL_Window* _window;

	Window();
	~Window();

	bool init(const int width, const int height);

	void handleWindowEvent(SDL_Event& e);

	const unsigned int getWidth() const;
	const unsigned int getHeight() const;
	const bool getMouseFocus() const;
	const bool getKeyboardFocus() const;
	const bool getFullscreen() const;
	const bool getMinimized() const;
	const bool getQuit() const;
	void setQuit(bool val);
};