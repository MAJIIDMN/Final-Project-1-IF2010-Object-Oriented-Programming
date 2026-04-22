#ifndef CORE_STATE_GAME_STATE_VIEW_HPP
#define CORE_STATE_GAME_STATE_VIEW_HPP

#include <string>
#include <vector>

#include "core/state/header/GameState.hpp"
#include "utils/Types.hpp"

class GameStateView {
public:
	GameStateView();
	explicit GameStateView(const GameState& state);

	void refresh(const GameState& state);

	int getCurrentTurn() const;
	int getMaxTurn() const;
	const std::string& getActivePlayerName() const;
	int getActivePlayerIndex() const;
	const std::vector<PlayerView>& getPlayers() const;
	const std::vector<PropertyView>& getProperties() const;
	bool getHasRolledDice() const;
	bool getHasUsedSkillCard() const;

	std::string render(const GameState& state) const;

private:
	int currentTurn;
	int maxTurn;
	std::string activePlayerName;
	int activePlayerIndex;
	std::vector<PlayerView> playerViews;
	std::vector<PropertyView> propertyViews;
	bool hasRolledDice;
	bool hasUsedSkillCard;

protected:
};

#endif
