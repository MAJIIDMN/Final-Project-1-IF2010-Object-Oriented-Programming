#ifndef MANAGER_HEADER_BANKRUPTCY_MANAGER_HPP
#define MANAGER_HEADER_BANKRUPTCY_MANAGER_HPP

#include <vector>
#include "models/Money.hpp"
#include "ui/UiDtos.hpp"

class Player;
class Board;
class Bank;
class FestivalManager;
class TransactionLogger;
class GameState;
class TurnManager;
class AuctionManager;
class PropertyTile;
class EventBus;

class BankruptcyManager {
public:
	BankruptcyManager();

	void setBankruptcyThreshold(int threshold);
	bool isBankrupt(int balance) const;

	void execute(Player& debtor, Player* creditor, Money obligation,
		GameState& gameState, Board& board, Bank& bank,
		FestivalManager& festivalManager, TransactionLogger& logger,
		TurnManager& turnManager, AuctionManager& auctionManager,
		EventBus& eventBus);

private:
	int bankruptcyThreshold;

	LiquidationState buildLiquidationStateSnapshot(const Player& debtor, const Board& board, Money obligation);
};

#endif
