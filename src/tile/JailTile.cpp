#include "tile/header/JailTile.hpp"

JailTile::JailTile(int id, const std::string& code, const std::string& name, int fine)
	: SpecialTile(id, code, name, TileType::JAIL), fine(fine) {}

int JailTile::getFine() const {
	return fine;
}
