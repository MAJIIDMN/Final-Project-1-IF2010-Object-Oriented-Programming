#ifndef TILE_HEADER_ACTION_TILE_HPP
#define TILE_HEADER_ACTION_TILE_HPP

#include "tile/header/Tile.hpp"
#include "utils/Enums.hpp"

class ActionTile : public Tile {
public:
	ActionTile(int id, const std::string& code, const std::string& name, TileType type);
};

#endif
