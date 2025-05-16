#pragma once

#include "math/arrayEx.hpp"
#include "enums/itemTypes.hpp"
#include <nlohmann/json.hpp>

#include <vector>
#include <string>

template<typename T>
inline void to_json(nlohmann::json& j, const Array<T>& a) {
	j = nlohmann::json{ a.x, a.y };
}

template<typename T>
inline void from_json(const nlohmann::json& j, Array<T>& a) {
	j.at(0).get_to(a.x);
	j.at(1).get_to(a.y);
}

struct ActorSnapshot {
	std::string type;
	Float2 pos;
	Float2 velocity;
	int health;
	float angle;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ActorSnapshot, type, pos, velocity, health, angle);
};

struct ObjectSnapshot {
	Float2 pos;
	Float2 velocity;
	Int2 src;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ObjectSnapshot, pos, velocity, src);
};

struct DestructibleSnapshot {
	Float2 pos;
	Float2 velocity;
	int health;
	Int2 src;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DestructibleSnapshot, pos, velocity, health, src);
};

struct ItemSnapshot {
	ItemType type;
	Float2 pos;
	bool pickedUp;
	Int2 src;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemSnapshot, type, pos, pickedUp, src);
};

struct SaveData {
	int version = 1;
	std::string level;
	float timer;
	std::vector<ActorSnapshot> actors;
	std::vector<ObjectSnapshot> objects;
	std::vector<DestructibleSnapshot> destructibles;
	std::vector<ItemSnapshot> items;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveData, version, level, timer, actors, objects, destructibles, items);
};