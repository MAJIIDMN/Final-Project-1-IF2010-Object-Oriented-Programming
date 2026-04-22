#include "core/state/header/GameState.hpp"

GameState::GameState() : phase(GamePhase::SETUP), turn(0) {}

GamePhase GameState::getPhase() const {
	return phase;
}

int GameState::getTurn() const {
	return turn;
}

void GameState::setPhase(GamePhase value) {
	phase = value;
}

void GameState::nextTurn() {
	++turn;
}
