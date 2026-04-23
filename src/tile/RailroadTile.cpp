#include "tile/header/RailroadTile.hpp"
#include "models/Player.hpp"

RailroadTile::RailroadTile(int id, const std::string& code, const std::string& name,
						   Money price, Money mortgageValue)
	: PropertyTile(id, code, name, TileType::RAILROAD, price, mortgageValue) {}

Money RailroadTile::getRent(int /*diceRoll*/) const {
	if (isMortgaged() || !getOwner())
		return Money::zero();
	int count = getOwner()->countRailroads();
	// Rent values: 1=25, 2=50, 3=100, 4=200
	int rent = 0;
	switch (count) {
		case 1: rent = 25; break;
		case 2: rent = 50; break;
		case 3: rent = 100; break;
		case 4: rent = 200; break;
		default: rent = 0; break;
	}
	return Money(rent);
}
