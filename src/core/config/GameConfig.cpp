#include "core/config/header/GameConfig.hpp"

GameConfig::GameConfig() : playerCount(4), maxTurns(100), startingMoney(1000000) {}

int GameConfig::getPlayerCount() const {
	return playerCount;
}

int GameConfig::getMaxTurns() const {
	return maxTurns;
}

int GameConfig::getStartingMoney() const {
	return startingMoney;
}

void GameConfig::setPlayerCount(int value) {
	playerCount = value;
}

void GameConfig::setMaxTurns(int value) {
	maxTurns = value;
}

void GameConfig::setStartingMoney(int value) {
	startingMoney = value;
}
