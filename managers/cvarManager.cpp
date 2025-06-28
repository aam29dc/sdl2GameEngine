#include "cvarManager.hpp"
#include <iostream>
#include <string>
#include <algorithm>

CvarManager& CvarManager::getInstance() {
	static CvarManager instance;
	return instance;
}

CvarManager::~CvarManager() {
	for (auto& pair : cvars) {
		delete pair.second;  // delete the pointer
		pair.second = nullptr;
	}
	cvars.clear();
}

void CvarManager::registerCvar(BaseCvar* cvar) {
	if (!cvar) return;
	const std::string& name = cvar->getName();
	if (cvars.find(name) != cvars.end()) {
		std::cerr << "CVar already registered: " << name << "\n";
		return;
	}
	cvars[name] = cvar;
}

BaseCvar* CvarManager::getRawCvar(const std::string& name) {
	auto it = cvars.find(name);
	if (it != cvars.end()) return it->second;

	std::cout << "[getRawCvar] Failed lookup: [" << name << "]\n";
	std::cout << "Available CVars:\n";
	for (const auto& pair : cvars) {
		std::cout << " - [" << pair.first << "]\n";
	}

	return nullptr;
}

BaseCvar* CvarManager::getCvar(const std::string& name) {
	auto raw = getRawCvar(name);
	if (!raw) {
		std::cout << "CVar [" << name << "] not found in map.\n";
		for (const auto& pair : cvars) {
			std::cout << "- Registered CVar: [" << pair.first << "]\n";
		}
		return nullptr;
	}

	// Rest as-is...
	switch (raw->getType()) {
	case CvarType::Float: return static_cast<Cvar<float>*>(raw);
	case CvarType::Int: return static_cast<Cvar<int>*>(raw);
	case CvarType::Bool: return static_cast<Cvar<bool>*>(raw);
	case CvarType::String: return static_cast<Cvar<std::string>*>(raw);
	}

	return nullptr;
}

size_t CvarManager::size() const {
	return cvars.size();
}

void CvarManager::list() const {
	for (auto& pair : cvars) {
		if (pair.second) {
			std::cout << " Name: " << pair.first << "\n";
		}
	}
}