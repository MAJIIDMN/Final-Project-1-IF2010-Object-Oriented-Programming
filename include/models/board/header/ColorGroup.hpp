#ifndef MODELS_BOARD_HEADER_COLOR_GROUP_HPP
#define MODELS_BOARD_HEADER_COLOR_GROUP_HPP

#include <vector>
#include "utils/Enums.hpp"

class StreetTile;

class ColorGroup {
public:
	ColorGroup(Color color);

	Color getColor() const;
	const std::vector<StreetTile*>& getStreets() const;

	void addStreet(StreetTile* street);
	bool isMonopolized() const;
	bool hasUniformBuildLevel() const;
	int getMinBuildLevel() const;

private:
	Color color;
	std::vector<StreetTile*> streets;
};

#endif
