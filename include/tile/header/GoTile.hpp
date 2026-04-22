#ifndef TILE_HEADER_GO_TILE_HPP
#define TILE_HEADER_GO_TILE_HPP

#include "tile/header/SpecialTile.hpp"
#include "utils/Enums.hpp"

class GoTile : public SpecialTile {
public:
	GoTile(int id, const std::string& code, const std::string& name, int salary);

	int getSalary() const;

private:
	int salary;
};

#endif
