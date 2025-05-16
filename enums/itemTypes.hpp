#pragma once

#include <iostream>

enum class ItemType {
	Empty,
	LesserHeal,
	Heal,
	Mana,
	Speed,
};

namespace ItemEffects {
	constexpr int Empty = 0;
	constexpr int LesserHeal = 25;
	constexpr int Heal = 50;
	constexpr int Mana = 30;
	constexpr float SpeedMultiplier = 2.0f;
	constexpr float SpeedDuration = 5.0f;
}

inline std::string to_string(ItemType item) {
	switch (item) {
	case ItemType::Empty: return "Empty";
	case ItemType::LesserHeal: return "LesserHeal";
	case ItemType::Heal: return "Heal";
	case ItemType::Mana: return "Mana";
	case ItemType::Speed: return "Speed";
	default: return "UNKNOWN";
	}
}

inline std::ostream& operator<<(std::ostream& os, ItemType item) {
	return os << to_string(item);
}