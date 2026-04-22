#include "tile/header/ChanceTile.hpp"

ChanceTile::ChanceTile(int id, const std::string& code, const std::string& name)
	: ActionTile(id, code, name, TileType::CHANCE) {}
