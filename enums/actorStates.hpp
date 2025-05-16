#pragma once

#include <iostream>

enum class Commands {
	FORWARD,
	BACKWARD,
	LEFTWARD,
	RIGHTWARD,
	ATTACKONE,
	ATTACKTWO,
	JUMP,
};

enum class States {
	IDLE,
	ATTACKING,
	MOVING,
	MOVINGATTACK,
	ALIVE,
	DEAD
};

inline std::string to_string(States state) {
	switch (state) {
	case States::IDLE: return "IDLE";
	case States::MOVING: return "MOVING";
	case States::ATTACKING: return "ATTACKING";
	case States::MOVINGATTACK: return "MOVINGATTACK";
	default: return "UNKNOWN";
	}
}

inline std::ostream& operator<<(std::ostream& os, States state) {
	return os << to_string(state);
}