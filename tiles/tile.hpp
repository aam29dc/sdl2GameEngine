#pragma once

#include "math/arrayEx.hpp"

class Tile {
private:
	Int2 src;
	Float2 dst;
	bool walkable;
	bool visible;
public:
	Tile(const Int2& src, const Float2& dst, const bool walkable = true, const bool visible = true);
	~Tile();

	const Int2& getSrc() const;
	const Float2& getDst() const;

	const void setDst(const Float2& dst);

	bool getWalkable() const;
	bool getVisible() const;

	static constexpr int ISOWIDTH = 64;
	static constexpr int ISOHEIGHT = 64;

	static constexpr int WIDTH = 32;
	static constexpr int HEIGHT = 32;

	static constexpr int MARGIN = 1;
};