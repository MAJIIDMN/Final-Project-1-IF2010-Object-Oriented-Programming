#ifndef TILE_HEADER_COMMUNITY_CHEST_TILE_HPP
#define TILE_HEADER_COMMUNITY_CHEST_TILE_HPP

#include "tile/header/ActionTile.hpp"
#include "utils/Enums.hpp"

class CommunityChestTile : public ActionTile {
public:
	CommunityChestTile(int id, const std::string& code, const std::string& name);
};

#endif
