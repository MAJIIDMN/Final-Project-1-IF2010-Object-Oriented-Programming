#ifndef TILE_HEADER_FREE_PARKING_TILE_HPP
#define TILE_HEADER_FREE_PARKING_TILE_HPP

#include "tile/header/SpecialTile.hpp"
#include "utils/Enums.hpp"

class FreeParkingTile : public SpecialTile {
public:
	FreeParkingTile(int id, const std::string& code, const std::string& name);
};

#endif
