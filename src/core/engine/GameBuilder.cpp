#include "core/engine/header/GameBuilder.hpp"

GameBuilder::GameBuilder()
	: board(nullptr), turnManager(), auctionManager(), bankruptcyManager(), transactionLogger(), dice(),
	  gameConfig(), gameState() {}

GameBuilder& GameBuilder::withBoard(Board* value) {
	board = value;
	return *this;
}

GameBuilder& GameBuilder::withConfig(const GameConfig& value) {
	gameConfig = value;
	return *this;
}

GameEngine GameBuilder::build() {
	gameState.setPhase(GamePhase::SETUP);
	return GameEngine(gameState);
}

Board* GameBuilder::getBoard() const {
	return board;
}

TurnManager& GameBuilder::getTurnManager() {
	return turnManager;
}

AuctionManager& GameBuilder::getAuctionManager() {
	return auctionManager;
}

BankruptcyManager& GameBuilder::getBankruptcyManager() {
	return bankruptcyManager;
}

TransactionLogger& GameBuilder::getLogger() {
	return transactionLogger;
}

Dice& GameBuilder::getDice() {
	return dice;
}

GameConfig& GameBuilder::getConfig() {
	return gameConfig;
}

GameState& GameBuilder::getGameState() {
	return gameState;
}
