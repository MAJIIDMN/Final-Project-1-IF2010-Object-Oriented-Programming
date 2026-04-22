#include "tile/header/GoTile.hpp"

GoTile::GoTile(int id, const std::string& code, const std::string& name, int salary)
	: SpecialTile(id, code, name, TileType::GO), salary(salary) {}

int GoTile::getSalary() const {
	return salary;
}
