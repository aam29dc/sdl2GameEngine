#pragma once

#include "managers/saveData.hpp"

#include <fstream>
#include <optional>
#include "nlohmann/json.hpp"

class SaveManager {
public:
    static bool saveToFile(const SaveData& data, const std::string& path) {
        std::ofstream out(path);
        if (!out) return false;
        out << nlohmann::json(data).dump(4); // Serialize SaveData to JSON
        return true;
    }

    static std::optional<SaveData> loadFromFile(const std::string& path) {
        std::ifstream in(path);
        if (!in) return std::nullopt;
        nlohmann::json j;
        in >> j;
        return j.get<SaveData>(); // Deserialize JSON into SaveData
    }
};