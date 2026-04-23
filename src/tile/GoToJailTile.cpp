#include "tile/header/GoToJailTile.hpp"

GoToJailTile::GoToJailTile(int id, const std::string& code, const std::string& name)
	: SpecialTile(id, code, name, TileType::GO_TO_JAIL) {}
