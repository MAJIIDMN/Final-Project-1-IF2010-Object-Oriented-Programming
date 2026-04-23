#include "tile/header/UtilityTile.hpp"
#include "models/Player.hpp"

UtilityTile::UtilityTile(int id, const std::string& code, const std::string& name,
						 Money price, Money mortgageValue)
	: PropertyTile(id, code, name, TileType::UTILITY, price, mortgageValue) {}

Money UtilityTile::getRent(int diceRoll) const {
	if (isMortgaged() || !getOwner() || diceRoll <= 0)
		return Money::zero();
	int count = getOwner()->countUtilities();
	int factor = 0;
	switch (count) {
		case 1: factor = 4; break;
		case 2: factor = 10; break;
		default: factor = 0; break;
	}
	return Money(diceRoll * factor);
}
