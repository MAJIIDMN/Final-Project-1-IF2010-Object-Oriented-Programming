#include "core/engine/header/GameEngine.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

#include "controllers/PlayerController.hpp"
#include "models/Money.hpp"
#include "models/Player.hpp"
#include "models/event/header/GameEvent.hpp"
#include "core/state/header/GameStateView.hpp"

GameEngine::GameEngine(GameState initialState)
	: gameState(initialState), running(false), commandRegistry(), turnManager(), auctionManager(),
	  bankruptcyManager(), eventBus(), transactionLogger() {
	bankruptcyManager.setBankruptcyThreshold(0);
	eventBus.subscribe(&transactionLogger);
}

void GameEngine::initialize(const std::vector<Player*>& players, int maxTurn) {
	gameState.setPlayers(players);
	gameState.setTurnOrder(players);
	gameState.setActivePlayerIndex(0);
	gameState.setCurrentTurn(1);
	gameState.setMaxTurn(maxTurn);

	turnManager.setPlayerCount(static_cast<int>(players.size()));
	gameState.setCurrentTurn(turnManager.getTurnNumber());
}

void GameEngine::start() {
	running = true;
	gameState.setPhase(GamePhase::RUNNING);
}

void GameEngine::stop() {
	running = false;
	gameState.setPhase(GamePhase::FINISHED);
	eventBus.publish(GameOverEvent(gameState.getCurrentTurn(), "SYSTEM", "Game loop stopped"));
}

void GameEngine::runGameLoop() {
	if (gameState.getPlayers().empty()) {
		stop();
		return;
	}

	if (!isRunning()) {
		start();
	}

	while (running) {
		if (gameState.getMaxTurn() > 0 && gameState.getCurrentTurn() > gameState.getMaxTurn()) {
			stop();
			break;
		}

		const int activeIndex = gameState.getActivePlayerIndex();
		const auto& players = gameState.getPlayers();
		if (activeIndex < 0 || activeIndex >= static_cast<int>(players.size()) || players[activeIndex] == nullptr) {
			stop();
			break;
		}

		executeTurn(*players[activeIndex]);
	}
}

bool GameEngine::isRunning() const {
	return running;
}

GameState& GameEngine::getState() {
	return gameState;
}

const GameState& GameEngine::getState() const {
	return gameState;
}

CommandRegistry& GameEngine::getCommandRegistry() {
	return commandRegistry;
}

const CommandRegistry& GameEngine::getCommandRegistry() const {
	return commandRegistry;
}

TurnManager& GameEngine::getTurnManager() {
	return turnManager;
}

AuctionManager& GameEngine::getAuctionManager() {
	return auctionManager;
}

BankruptcyManager& GameEngine::getBankruptcyManager() {
	return bankruptcyManager;
}

EventBus& GameEngine::getEventBus() {
	return eventBus;
}

TransactionLogger& GameEngine::getTransactionLogger() {
	return transactionLogger;
}

bool GameEngine::processCommand(const std::string& input, Player& player) {
	const std::string commandToken = normalizeCommandToken(input);
	if (commandToken.empty()) {
		return false;
	}

	if (handleAuctionCommand(input, player)) {
		return true;
	}

	const bool executed = commandRegistry.parseAndExecute(input, *this, player);
	if (!executed) {
		transactionLogger.log(gameState.getCurrentTurn(), player.getUsername(), "INVALID_COMMAND", input);
		return false;
	}

	if (commandToken == "roll_dice") {
		eventBus.publish(PlayerMovedEvent(gameState.getCurrentTurn(), player.getUsername(), "Rolled dice"));
	}

	if (commandToken == "end_turn") {
		eventBus.publish(TurnEndedEvent(gameState.getCurrentTurn(), player.getUsername(), "Turn ended"));
		turnManager.nextTurn();
		gameState.setCurrentTurn(turnManager.getTurnNumber());
		gameState.setActivePlayerIndex(turnManager.getCurrentPlayerIndex());
	}

	checkBankruptcy(player);
	return true;
}

void GameEngine::executeTurn(Player& player) {
	eventBus.publish(TurnStartedEvent(gameState.getCurrentTurn(), player.getUsername(), "Turn started"));

	const GameStateView stateView = gameState.toView();
	PlayerController* controller = player.getController();
	if (controller == nullptr) {
		stop();
		return;
	}

	const std::string command = controller->chooseCommand(stateView);
	if (!processCommand(command, player)) {
		processCommand("end_turn", player);
	}
}

bool GameEngine::handleAuctionCommand(const std::string& input, Player& player) {
	std::istringstream stream(input);
	std::string token;
	stream >> token;
	std::transform(token.begin(), token.end(), token.begin(),
		[](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

	if (token == "auction_start") {
		int openingBid = 0;
		stream >> openingBid;
		auctionManager.startAuction(openingBid);
		eventBus.publish(AuctionStartedEvent(gameState.getCurrentTurn(), player.getUsername(), "Auction opened"));
		return true;
	}

	if (token == "auction_bid") {
		int bid = 0;
		stream >> bid;
		if (auctionManager.placeBid(bid)) {
			eventBus.publish(BidPlacedEvent(gameState.getCurrentTurn(), player.getUsername(), "Bid accepted"));
			return true;
		}
		transactionLogger.log(gameState.getCurrentTurn(), player.getUsername(), "AUCTION_BID_REJECTED", input);
		return true;
	}

	if (token == "auction_close") {
		auctionManager.closeAuction();
		transactionLogger.log(gameState.getCurrentTurn(), player.getUsername(), "AUCTION_CLOSED", "Auction closed");
		return true;
	}

	return false;
}

void GameEngine::checkBankruptcy(Player& player) {
	if (bankruptcyManager.isBankrupt(player.getMoney().getAmount())) {
		player.setStatus(PlayerStatus::BANKRUPT);
		eventBus.publish(BankruptcyEvent(gameState.getCurrentTurn(), player.getUsername(), "Player bankrupt"));
	}
}

std::string GameEngine::normalizeCommandToken(const std::string& input) const {
	std::istringstream stream(input);
	std::string token;
	stream >> token;
	std::transform(token.begin(), token.end(), token.begin(),
		[](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });
	return token;
}
