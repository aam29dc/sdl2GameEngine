#pragma once

#include <SDL_mixer.h>

#include "math/arrayEx.hpp"

#include <string>
#include <map>
#include <queue>

class Camera;

enum soundType {
	SOUND_MUSIC = 0,
	SOUND_SFX = 1,
	SOUND_UI = 2,
};

class SoundManager {
private:
	std::map<std::string, Mix_Chunk*> sfxsMap;
	std::map<std::string, Mix_Chunk*> UIMap;
	std::map<std::string, Mix_Music*> musicMap;
	std::queue<std::pair<std::string, Float2>> soundsQueue;

	const float maxDistanceSq = 1400.0f*1400.0f;
public:
	SoundManager();
	~SoundManager();

	bool init();

	bool load(const std::string& fileName, const std::string& id, const soundType type);
	bool playSfx(const std::pair<std::string, Float2>& sound, const Camera* const camera, const int loop = false);
	bool playUI(const std::pair<std::string, Float2>& sound, const int loop);
	bool playMusic(const std::string& id, const int loop = false);

	void clear();

	void push(const std::string& id, const Float2& pos = {0});
	void update(const Camera* const camera);
};