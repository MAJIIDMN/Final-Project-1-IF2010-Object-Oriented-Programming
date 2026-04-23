#ifndef TILE_HEADER_RAILROAD_TILE_HPP
#define TILE_HEADER_RAILROAD_TILE_HPP

#include "tile/header/PropertyTile.hpp"
#include "utils/Enums.hpp"

class RailroadTile : public PropertyTile {
public:
	RailroadTile(int id, const std::string& code, const std::string& name,
				 Money price, Money mortgageValue);

	Money getRent(int diceRoll = 0) const override;
};

#endif
