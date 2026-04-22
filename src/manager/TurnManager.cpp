#include "manager/header/TurnManager.hpp"

TurnManager::TurnManager(int playerCount) : playerCount(playerCount), currentPlayerIndex(0), turnNumber(1) {}

int TurnManager::getCurrentPlayerIndex() const {
	return currentPlayerIndex;
}

int TurnManager::getTurnNumber() const {
	return turnNumber;
}

int TurnManager::getPlayerCount() const {
	return playerCount;
}

void TurnManager::setPlayerCount(int count) {
	playerCount = count < 0 ? 0 : count;
}

void TurnManager::nextTurn() {
	if (playerCount <= 0) {
		return;
	}
	currentPlayerIndex = (currentPlayerIndex + 1) % playerCount;
	++turnNumber;
}
