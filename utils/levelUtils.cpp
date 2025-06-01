#include "utils/levelUtils.hpp"

const Int2 convertToItemSrc(const char ch, const Int2& size, const int margin) {
	// 0 to 5 (empty, lesser heal, major heal, mana potion, swiftness potion)
	// each row a differnt item
	int c = ch - 48;

	//int row = c / 8;
	//return {1, (size.y * row) + (row * margin) + 1	};
	return { (1 * c) + (32 * c) + 1, 1 };
}

const Int2 convertToSrc(const char ch, const Int2& size, const int margin) {
	// (48) 0 - 9, :, ;, <, =, >, ?, @, A-O (111)
	int c = ch - 48;

	int row = c / 8;
	int col = c % 8;

	return {
		(size.x * col) + (col * margin) + 1,
		(size.y * row) + (row * margin) + 1
	};
}