#ifndef MANAGER_HEADER_TURN_MANAGER_HPP
#define MANAGER_HEADER_TURN_MANAGER_HPP

class TurnManager {
public:
	explicit TurnManager(int playerCount = 0);

	int getCurrentPlayerIndex() const;
	int getTurnNumber() const;
	int getPlayerCount() const;

	void setPlayerCount(int count);
	void nextTurn();

private:
	int playerCount;
	int currentPlayerIndex;
	int turnNumber;

protected:
};

#endif
