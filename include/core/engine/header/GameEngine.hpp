#ifndef CORE_ENGINE_HEADER_GAME_ENGINE_HPP
#define CORE_ENGINE_HEADER_GAME_ENGINE_HPP

#include "core/state/header/GameState.hpp"

class GameEngine {
public:
	explicit GameEngine(GameState initialState = GameState());

	void start();
	void stop();

	bool isRunning() const;
	GameState& getState();
	const GameState& getState() const;

private:
	GameState gameState;
	bool running;

protected:
};

#endif
