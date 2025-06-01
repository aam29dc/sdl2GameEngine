#include <unordered_map>
#include <string>
#include "core/cvar.hpp"

class CvarManager {
private:
	std::unordered_map<std::string, BaseCvar*> cvars;
	static CvarManager* instance;

	CvarManager() = default;

public:
	static CvarManager* getInstance() {
		if (!instance)
			instance = new CvarManager();
		return instance;
	}

	void registerCvar(BaseCvar* cvar) {
		cvars[cvar->getName()] = cvar;
	}

	BaseCvar* getRawCvar(const std::string& name) {
		auto it = cvars.find(name);
		return it != cvars.end() ? it->second : nullptr;
	}

	BaseCvar* getCvar(const std::string& name) {
		BaseCvar* raw = getRawCvar(name);
		switch (raw->getType()) {
		case CvarType::Float:
			return static_cast<Cvar<float>*>(raw);
		case CvarType::Int:
			return static_cast<Cvar<int>*>(raw);
		case CvarType::Bool:
			return static_cast<Cvar<bool>*>(raw);
		case CvarType::String:
			return static_cast<Cvar<std::string>*>(raw);
		}
		return nullptr;
	}
};

CvarManager* CvarManager::instance = nullptr;