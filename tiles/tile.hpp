#pragma once

#include "math/arrayEx.hpp"

class Tile {
private:
	Int2 src;
	Int2 dst;
	bool walkable;
	bool visible;
public:
	Tile(const Int2& src, const Int2& dst, const bool walkable = true, const bool visible = true);
	~Tile();

	const Int2& getSrc() const;
	const Int2& getDst() const;

	const void setDst(const Int2& dst);

	bool getWalkable() const;
	bool getVisible() const;

	static constexpr int WIDTH = 32;
	static constexpr int HEIGHT = 32;
	static constexpr int MARGIN = 1;
};