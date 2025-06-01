#include "tile.hpp"

Tile::Tile(const Int2& src, const Float2& dst, const bool walkable, const bool visible) {
	this->src = src;
	this->dst = dst;
	this->walkable = walkable;
	this->visible = visible;
}

Tile::~Tile() {

}

const Int2& Tile::getSrc() const {
	return src;
}

const Float2& Tile::getDst() const {
	return dst;
}

const void Tile::setDst(const Float2& dst) {
	this->dst = dst;
}

bool Tile::getWalkable() const {
	return walkable;
}

bool Tile::getVisible() const {
	return visible;
}