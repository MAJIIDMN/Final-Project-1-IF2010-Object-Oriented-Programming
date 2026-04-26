#include "models/board/header/ColorGroup.hpp"
#include "tile/header/StreetTile.hpp"

ColorGroup::ColorGroup(Color color) : color(color) {}

Color ColorGroup::getColor() const {
	return color;
}

const std::vector<StreetTile*>& ColorGroup::getStreets() const {
	return streets;
}

void ColorGroup::addStreet(StreetTile* street) {
	streets.push_back(street);
}

bool ColorGroup::isMonopolized() const {
	if (streets.empty())
		return false;
	Player* firstOwner = streets[0]->getOwner();
	if (!firstOwner)
		return false;
	for (const auto* street : streets) {
		if (street->getOwner() != firstOwner || street->isMortgaged())
			return false;
	}
	return true;
}

bool ColorGroup::hasUniformBuildLevel() const {
	if (streets.size() <= 1)
		return true;
	int firstLevel = streets[0]->getBuildingLevel();
	for (const auto* street : streets) {
		if (street->getBuildingLevel() != firstLevel)
			return false;
	}
	return true;
}

int ColorGroup::getMinBuildLevel() const {
	if (streets.empty())
		return 0;
	int minLevel = streets[0]->getBuildingLevel();
	for (const auto* street : streets) {
		if (street->getBuildingLevel() < minLevel)
			minLevel = street->getBuildingLevel();
	}
	return minLevel;
}

bool ColorGroup::satisfiesEvenBuildRule(const StreetTile& street) const {
	const int level = street.getBuildingLevel();
	if (level < 4) {
		return level == getMinBuildLevel();
	}
	if (level == 4) {
		return hasUniformBuildLevel();
	}
	return false;
}

void ColorGroup::demolishAllBuildings() {
	for (StreetTile* street : streets) {
		while (street && street->getBuildingLevel() > 0) {
			street->demolish();
		}
	}
}
