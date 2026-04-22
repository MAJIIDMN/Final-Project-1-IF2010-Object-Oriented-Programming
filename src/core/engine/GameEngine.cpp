#include "core/engine/header/GameEngine.hpp"

GameEngine::GameEngine(GameState initialState) : gameState(initialState), running(false) {}

void GameEngine::start() {
	running = true;
	gameState.setPhase(GamePhase::RUNNING);
}

void GameEngine::stop() {
	running = false;
	gameState.setPhase(GamePhase::FINISHED);
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
