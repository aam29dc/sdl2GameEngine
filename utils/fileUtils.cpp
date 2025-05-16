#include "utils/fileUtils.hpp"

int getMaxLineLength(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return -1;
	}

	std::string line;
	int maxLength = 0;

	while (std::getline(file, line)) {
		if (line.length() > maxLength) {
			maxLength = static_cast<int>(line.length());
		}
	}

	file.close();
	return maxLength;
}