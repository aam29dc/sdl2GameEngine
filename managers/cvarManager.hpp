#include <unordered_map>
#include <string>
#include "core/cvar.hpp"

class CvarManager {
private:
	std::unordered_map<std::string, BaseCvar*> cvars;
	static CvarManager* instance;

	CvarManager() = default;
public:
	static CvarManager& getInstance();
	~CvarManager();
	void registerCvar(BaseCvar* cvar);
	BaseCvar* getRawCvar(const std::string& name);
	BaseCvar* getCvar(const std::string& name);

	void list() const;

	size_t size() const;
};