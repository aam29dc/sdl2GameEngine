#include "managers/soundManager.hpp"
#include "core/camera.hpp"
#include <algorithm>
#include <iostream>

SoundManager::SoundManager() {

}

SoundManager::~SoundManager() {
	for (auto& m : musicMap) {
		delete &m;
	}

	for (auto& s : sfxsMap) {
		delete& s;
	}

	for (auto& u : UIMap) {
		delete& u;
	}

	musicMap.clear();
	sfxsMap.clear();
	UIMap.clear();
}

bool SoundManager::init() {
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		#ifdef DEBUG
			std::cout << "SDL_mixer could not initialize! SDL_mixer Error: %s\n" << Mix_GetError() << "\n";
		#endif
		return false;
	}

	Mix_Volume(-1, 32);
	return true;
}

bool SoundManager::load(const std::string& fileName, const std::string& id, const soundType type) {
	if (type == SOUND_MUSIC) {
		if (musicMap.find(id) != musicMap.end()) {
			std::cout << "Music with that id: " << id << " already exists." << std::endl;
			return false;
		}
		Mix_Music* musicFile = Mix_LoadMUS(fileName.c_str());

		if (musicFile == nullptr) {
			std::cout << "Couldn't load music: " << Mix_GetError() << std::endl;
			return false;
		}
		musicMap[id] = musicFile;
		return true;
	}
	else if (type == SOUND_SFX) {
		Mix_Chunk* chunkFile = Mix_LoadWAV(fileName.c_str());
		if (sfxsMap.find(id) != sfxsMap.end()) {
			std::cout << "Sound with that id: " << id << " already exists." << std::endl;
			return false;
		}

		if (chunkFile == nullptr) {
			std::cout << "Couldn't load sfx: " << Mix_GetError() << std::endl;
			return false;
		}
		sfxsMap[id] = chunkFile;
		return true;
	}
	return false;
}

bool SoundManager::playSfx(const std::pair<std::string, Float2>& sound, const Camera* const camera, const int loop) {
	auto it = sfxsMap.find(sound.first);
	if (it != sfxsMap.end()) {
		Float2 soundPos = sound.second; // world space
		float dx = soundPos.x - camera->getPos().x;
		float dy = soundPos.y - camera->getPos().y;

		float distSq = dx * dx + dy * dy;

		if (distSq < maxDistanceSq) {
			int volume = static_cast<int>((1.0f - (distSq / maxDistanceSq)) * MIX_MAX_VOLUME);
			volume = std::clamp(volume, 0, MIX_MAX_VOLUME);

			Mix_VolumeChunk(sfxsMap[sound.first], volume);
			Mix_PlayChannel(-1, sfxsMap[sound.first], loop);
		}
		return true;
	}
	std::cout << "Sfx: " << sound.first << " DNE." << std::endl;
	return false;
}

bool SoundManager::playUI(const std::pair<std::string, Float2>& sound, const int loop) {
	auto it = UIMap.find(sound.first);
	if (it != UIMap.end()) {
		Mix_VolumeChunk(UIMap[sound.first], MIX_MAX_VOLUME);
		Mix_PlayChannel(-1, UIMap[sound.first], loop);
		return true;
	}
	std::cout << "UI sound: " << sound.first << " DNE." << std::endl;
	return false;
}

bool SoundManager::playMusic(const std::string& id, const int loop) {
	auto it = musicMap.find(id);
	if (it != musicMap.end()) {
		Mix_PlayMusic(musicMap[id], loop);
		return true;
	}
	std::cout << "Music: " << id << " DNE." << std::endl;
	return false;
}

void SoundManager::clear() {
	for (auto& s : sfxsMap) {
		Mix_FreeChunk(s.second);
	}
	sfxsMap.clear();

	for (auto& m : musicMap) {
		Mix_FreeMusic(m.second);
	}
	musicMap.clear();

	while (!soundsQueue.empty()) {
		soundsQueue.pop();
	}
}

void SoundManager::push(const std::string& id, const Float2 &pos) {
	soundsQueue.push({id, pos});
}

void SoundManager::update(const Camera* const camera) {
	while (!soundsQueue.empty()) {
		playSfx(soundsQueue.front(), camera);
		soundsQueue.pop();
	}
}