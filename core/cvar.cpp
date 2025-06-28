#include "core/cvar.hpp"

std::string BaseCvar::extractCvarValue(const std::string& input) {
    size_t equalPos = input.find('=');
    if (equalPos != std::string::npos) {
        return input.substr(equalPos + 1);
    }

    size_t spacePos = input.find(' ');
    if (spacePos != std::string::npos) {
        return input.substr(spacePos + 1);
    }

    return "";
}