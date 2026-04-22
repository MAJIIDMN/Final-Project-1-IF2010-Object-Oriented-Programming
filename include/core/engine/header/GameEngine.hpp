#ifndef CORE_ENGINE_HEADER_GAME_ENGINE_HPP
#define CORE_ENGINE_HEADER_GAME_ENGINE_HPP

#include <string>
#include <vector>

#include "core/command/header/CommandRegistry.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "core/state/header/GameState.hpp"
#include "manager/header/AuctionManager.hpp"
#include "manager/header/BankruptcyManager.hpp"
#include "manager/header/EventBus.hpp"
#include "manager/header/TurnManager.hpp"

class Player;

class GameEngine {
public:
	explicit GameEngine(GameState initialState = GameState());

	void initialize(const std::vector<Player*>& players, int maxTurn);
	void start();
	void stop();
	void runGameLoop();

	bool isRunning() const;
	GameState& getState();
	const GameState& getState() const;

	CommandRegistry& getCommandRegistry();
	const CommandRegistry& getCommandRegistry() const;
	TurnManager& getTurnManager();
	AuctionManager& getAuctionManager();
	BankruptcyManager& getBankruptcyManager();
	EventBus& getEventBus();
	TransactionLogger& getTransactionLogger();
	bool processCommand(const std::string& input, Player& player);

private:
	void executeTurn(Player& player);
	bool handleAuctionCommand(const std::string& input, Player& player);
	void checkBankruptcy(Player& player);
	std::string normalizeCommandToken(const std::string& input) const;

	GameState gameState;
	bool running;
	CommandRegistry commandRegistry;
	TurnManager turnManager;
	AuctionManager auctionManager;
	BankruptcyManager bankruptcyManager;
	EventBus eventBus;
	TransactionLogger transactionLogger;

protected:
};

#endif
