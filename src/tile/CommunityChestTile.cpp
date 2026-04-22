#include "tile/header/CommunityChestTile.hpp"

CommunityChestTile::CommunityChestTile(int id, const std::string& code, const std::string& name)
	: ActionTile(id, code, name, TileType::COMMUNITY_CHEST) {}
