#include "tile/header/PropertyTile.hpp"
#include "models/Player.hpp"

PropertyTile::PropertyTile(int id, const std::string& code, const std::string& name,
						   TileType type, Money price, Money mortgageValue)
	: Tile(id, code, name, type), price(price), mortgageValue(mortgageValue),
	  owner(nullptr), status(PropertyStatus::BANK) {}

Money PropertyTile::getPrice() const {
	return price;
}

Money PropertyTile::getMortgageValue() const {
	return mortgageValue;
}

Player* PropertyTile::getOwner() const {
	return owner;
}

void PropertyTile::setOwner(Player* player) {
	owner = player;
}

PropertyStatus PropertyTile::getStatus() const {
	return status;
}

void PropertyTile::setStatus(PropertyStatus s) {
	status = s;
}

bool PropertyTile::isOwned() const {
	return status == PropertyStatus::OWNED;
}

bool PropertyTile::isMortgaged() const {
	return status == PropertyStatus::MORTGAGED;
}

bool PropertyTile::canBeDeveloped() const {
	return false;
}

int PropertyTile::getBuildingLevel() const {
	return 0;
}
