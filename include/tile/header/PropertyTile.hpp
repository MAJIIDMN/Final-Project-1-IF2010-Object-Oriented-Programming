#ifndef TILE_HEADER_PROPERTY_TILE_HPP
#define TILE_HEADER_PROPERTY_TILE_HPP

#include "tile/header/Tile.hpp"
#include "models/Money.hpp"
#include "utils/Enums.hpp"

class Player;
class Board;
class PropertyInfo;

class PropertyTile : public Tile {
public:
	PropertyTile(int id, const std::string& code, const std::string& name, TileType type,
				 Money price, Money mortgageValue);

	Money getPrice() const;
	Money getMortgageValue() const;

	Player* getOwner() const;
	void setOwner(Player* player);

	PropertyStatus getStatus() const;
	void setStatus(PropertyStatus status);

	bool isOwned() const;
	bool isMortgaged() const;

	virtual Money getRent(int diceRoll = 0) const = 0;
	virtual bool canBeDeveloped() const;
	virtual int getBuildingLevel() const;

	void onLand(Player& player, GameContext& ctx, int diceTotal = 0) override;
	std::string getLabel() const override;
	PropertyInfo toInfo() const;
	Money getSaleValue() const;
	bool canMortgageNow(const Board& board) const;

private:
	Money price;
	Money mortgageValue;
	Player* owner;
	PropertyStatus status;
};

#endif
