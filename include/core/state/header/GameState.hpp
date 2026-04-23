#ifndef CORE_STATE_GAME_STATE_HPP
#define CORE_STATE_GAME_STATE_HPP

#include <vector>

class Player;
class PropertyTile;
class GameStateView;

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
	int getCurrentTurn() const;
	int getMaxTurn() const;
	const std::vector<Player*>& getPlayers() const;
	Player* getActivePlayer() const;
	int getActivePlayerIndex() const;
	const std::vector<Player*>& getTurnOrder() const;
	bool getHasRolledDice() const;
	bool getHasUsedSkillCard() const;
	int getConsecutiveDoubles() const;

	void setPhase(GamePhase value);
	void setCurrentTurn(int value);
	void setMaxTurn(int value);
	void setPlayers(const std::vector<Player*>& value);
	void setActivePlayerIndex(int value);
	void setTurnOrder(const std::vector<Player*>& value);
	void setHasRolledDice(bool value);
	void setHasUsedSkillCard(bool value);
	void incrementConsecutiveDoubles();
	void resetConsecutiveDoubles();
	void resetTurnFlags();
	bool canSave() const;
	GameStateView toView() const;
	void nextTurn();

private:
	GamePhase phase;
	int currentTurn;
	int maxTurn;
	std::vector<Player*> players;
	std::vector<Player*> turnOrder;
	int activePlayerIndex;
	std::vector<PropertyTile*> propertyStates;
	bool hasRolledDice;
	bool hasUsedSkillCard;
	int consecutiveDoubles;

protected:
};

#endif
