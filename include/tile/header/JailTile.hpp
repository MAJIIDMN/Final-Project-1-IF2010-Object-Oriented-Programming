#ifndef TILE_HEADER_JAIL_TILE_HPP
#define TILE_HEADER_JAIL_TILE_HPP

#include "tile/header/SpecialTile.hpp"
#include "utils/Enums.hpp"

class JailTile : public SpecialTile {
public:
	JailTile(int id, const std::string& code, const std::string& name, int fine);

	int getFine() const;

private:
	int fine;
};

#endif
