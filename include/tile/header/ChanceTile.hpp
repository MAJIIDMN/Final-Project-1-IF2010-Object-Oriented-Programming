#ifndef TILE_HEADER_CHANCE_TILE_HPP
#define TILE_HEADER_CHANCE_TILE_HPP

#include "tile/header/ActionTile.hpp"
#include "utils/Enums.hpp"

class ChanceTile : public ActionTile {
public:
	ChanceTile(int id, const std::string& code, const std::string& name);
};

#endif
