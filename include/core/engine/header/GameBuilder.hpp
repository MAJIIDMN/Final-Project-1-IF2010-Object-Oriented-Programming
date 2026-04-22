#ifndef CORE_ENGINE_HEADER_GAME_BUILDER_HPP
#define CORE_ENGINE_HEADER_GAME_BUILDER_HPP

#include "core/config/header/GameConfig.hpp"
#include "core/engine/header/GameEngine.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "core/state/header/GameState.hpp"
#include "manager/header/AuctionManager.hpp"
#include "manager/header/BankruptcyManager.hpp"
#include "manager/header/TurnManager.hpp"
#include "models/board/header/Board.hpp"
#include "models/board/header/Dice.hpp"

class GameBuilder {
public:
	GameBuilder();

	GameBuilder& withBoard(Board* value);
	GameBuilder& withConfig(const GameConfig& value);

	GameEngine build();

	Board* getBoard() const;
	TurnManager& getTurnManager();
	AuctionManager& getAuctionManager();
	BankruptcyManager& getBankruptcyManager();
	TransactionLogger& getLogger();
	Dice& getDice();
	GameConfig& getConfig();
	GameState& getGameState();

private:
	Board* board;
	TurnManager turnManager;
	AuctionManager auctionManager;
	BankruptcyManager bankruptcyManager;
	TransactionLogger transactionLogger;
	Dice dice;
	GameConfig gameConfig;
	GameState gameState;

protected:
};

#endif
