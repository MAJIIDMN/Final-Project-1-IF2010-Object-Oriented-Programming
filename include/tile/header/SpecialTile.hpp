#ifndef TILE_HEADER_SPECIAL_TILE_HPP
#define TILE_HEADER_SPECIAL_TILE_HPP

#include "tile/header/Tile.hpp"
#include "utils/Enums.hpp"

class SpecialTile : public Tile {
public:
	SpecialTile(int id, const std::string& code, const std::string& name, TileType type);
};

#endif
