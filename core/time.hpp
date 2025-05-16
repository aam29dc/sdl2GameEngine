#pragma once

#include <SDL_timer.h>

class Time {
private:
	static Time* instance;
	static Uint64 startTime;
	static Uint64 currentTime;
	static Uint64 lastTime;
	static double elapsedTime;
	static float timescale;
	static float dt;
public:
	Time();
	~Time();

	static Time* getInstance();

	void tick();

	void setStartTime();
	Uint64 getStartTime() const;

	void setCurrentTime();
	Uint64 getCurrentTime() const;

	void setLastTime();
	Uint64 getLastTime() const;

	void setElapsedTime();
	double getElapsedTime() const;

	void setDt();
	float getDt() const;
};