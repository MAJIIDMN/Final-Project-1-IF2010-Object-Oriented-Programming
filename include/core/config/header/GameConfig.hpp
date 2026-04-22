#ifndef CORE_CONFIG_HEADER_GAME_CONFIG_HPP
#define CORE_CONFIG_HEADER_GAME_CONFIG_HPP

class GameConfig {
public:
	GameConfig();

	int getPlayerCount() const;
	int getMaxTurns() const;
	int getStartingMoney() const;

	void setPlayerCount(int value);
	void setMaxTurns(int value);
	void setStartingMoney(int value);

private:
	int playerCount;
	int maxTurns;
	int startingMoney;

protected:
};

#endif
