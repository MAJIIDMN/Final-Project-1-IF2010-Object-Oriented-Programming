#ifndef TILE_HEADER_GO_TO_JAIL_TILE_HPP
#define TILE_HEADER_GO_TO_JAIL_TILE_HPP

#include "tile/header/SpecialTile.hpp"
#include "utils/Enums.hpp"

class GoToJailTile : public SpecialTile {
public:
	GoToJailTile(int id, const std::string& code, const std::string& name);
};

#endif
