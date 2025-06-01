#pragma once
#include <string>
#include <array>
#include <SDL_keycode.h>

enum class Movement : std::size_t {
	FORWARD,
	BACKWARD,
	LEFTWARD,
	RIGHTWARD,
	SIZE,
};

enum class Combat : std::size_t {
	ATTACKONE,
	SIZE,
};

enum class Interact : std::size_t {
	USE,
	INVENTORY,
	PAUSE,
	ITEM1,
	ITEM2,
	ITEM3,
	ITEM4,
	ITEM5,
	SIZE,
};

enum class Misc : std::size_t {
	ZOOMIN,
	ZOOMOUT,
	SAVE,
	CONSOLE,
	SIZE,
};

enum class InputType {
	Key,
	Mouse
};

struct InputBind {
	InputType type;
	union {
		SDL_Scancode key;
		Uint8 mouseButton;
	};

	InputBind() : type(InputType::Key), key(SDL_SCANCODE_UNKNOWN) {}
	InputBind(SDL_Scancode k) : type(InputType::Key), key(k) {}
	InputBind(Uint8 button) : type(InputType::Mouse), mouseButton(button) {}
};

template<typename E>
constexpr auto toIndex(const E e) {
	return static_cast<std::size_t>(e);
}

class Binds {
private:
	std::array<InputBind, toIndex(Movement::SIZE)> MoveBinds;
	std::array<InputBind, toIndex(Combat::SIZE)> CombatBinds;
	std::array<InputBind, toIndex(Interact::SIZE)> InteractBinds;
	std::array<InputBind, toIndex(Misc::SIZE)> MiscBinds;
public:
	Binds();
	virtual ~Binds() {}
	bool readBinds(const std::string& bindsFile);

	const InputBind& getMoveBind(Movement m) const;
	void setMoveBind(Movement m, const InputBind& bind);

	const InputBind& getCombatBind(Combat c) const;
	void setCombatBind(Combat c, const InputBind& bind);

	const InputBind& getInteractBind(Interact i) const;
	void setInteractBind(Interact i, const InputBind& bind);

	const InputBind& getMiscBind(Misc m) const;
	void setMiscBind(Misc m, const InputBind& bind);
};