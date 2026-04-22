#ifndef MODELS_BOARD_HEADER_BOARD_HPP
#define MODELS_BOARD_HEADER_BOARD_HPP

#include <memory>
#include <vector>
#include <string>

#include "utils/Enums.hpp"

class Tile;
class StreetTile;
class PropertyTile;
class ColorGroup;

class Board {
public:
	static constexpr int BOARD_SIZE = 40;

	Board();
	~Board();

	void addTile(std::unique_ptr<Tile> tile);
	Tile* getTile(int index) const;
	Tile* getTileByCode(const std::string& code) const;
	int getSize() const;

	StreetTile* getStreetByCode(const std::string& code) const;
	PropertyTile* getPropertyByCode(const std::string& code) const;

	void registerColorGroup(ColorGroup* group);
	ColorGroup* getColorGroup(Color color) const;
	void updateMonopolies();

private:
	std::vector<std::unique_ptr<Tile>> tiles;
	std::vector<ColorGroup*> colorGroups;
};

#endif
