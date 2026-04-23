#ifndef TILE_HEADER_UTILITY_TILE_HPP
#define TILE_HEADER_UTILITY_TILE_HPP

#include "tile/header/PropertyTile.hpp"
#include "utils/Enums.hpp"

class UtilityTile : public PropertyTile {
public:
	UtilityTile(int id, const std::string& code, const std::string& name,
				Money price, Money mortgageValue);

	Money getRent(int diceRoll) const override;
};

#endif
