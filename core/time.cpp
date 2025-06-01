#include "time.hpp"

Time* Time::instance = nullptr;
Uint64 Time::startTime = 0;
Uint64 Time::currentTime = 0;
Uint64 Time::lastTime = 0;
double Time::elapsedTime = 0;
float Time::timescale = 0;
float Time::dt = 0;

Time::Time() {
	startTime = 0;
	currentTime = 0;
	lastTime = 0;
	elapsedTime = 0;
	dt = 0.0;
	timescale = 1.0f;
}

Time::~Time() {
	instance = nullptr;
}

Time* Time::getInstance() {
	if (instance == nullptr) instance = new Time();
	return instance;
}

void Time::tick() {
	setLastTime();
	setCurrentTime();
	setDt();
	setElapsedTime();
}

void Time::setStartTime() {
	startTime = SDL_GetPerformanceCounter();
}

Uint64 Time::getStartTime() const {
	return startTime;
}

void Time::setCurrentTime() {
	currentTime = SDL_GetPerformanceCounter();
}

Uint64 Time::getCurrentTime() const {
	return currentTime;
}

void Time::setLastTime() {
	lastTime = currentTime;
}

Uint64 Time::getLastTime() const {
	return lastTime;
}

void Time::setElapsedTime() {
	elapsedTime = (double)(SDL_GetPerformanceCounter() - startTime) / SDL_GetPerformanceFrequency();
	elapsedTime *= timescale;
}

double Time::getElapsedTime() const {
	return elapsedTime;
}

void Time::setDt() {
	dt = (float)(currentTime - lastTime) / SDL_GetPerformanceFrequency();
	dt *= timescale;
}

float Time::getDt() const {
	return dt;
}

void Time::setTimeScale(const float& value) {
	timescale = value;
}

float Time::getTimeScale() const {
	return timescale;
}