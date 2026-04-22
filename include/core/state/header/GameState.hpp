#ifndef CORE_STATE_GAME_STATE_HPP
#define CORE_STATE_GAME_STATE_HPP

enum class GamePhase {
	SETUP,
	RUNNING,
	FINISHED,
};

class GameState {
public:
	GameState();

	GamePhase getPhase() const;
	int getTurn() const;

	void setPhase(GamePhase value);
	void nextTurn();

private:
	GamePhase phase;
	int turn;

protected:
};

#endif
