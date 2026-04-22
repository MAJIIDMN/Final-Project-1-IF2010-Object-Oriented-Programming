#include "core/state/header/GameStateView.hpp"

#include "models/Player.hpp"

GameStateView::GameStateView()
	: currentTurn(0), maxTurn(0), activePlayerName(""), activePlayerIndex(0), hasRolledDice(false),
	  hasUsedSkillCard(false) {}

GameStateView::GameStateView(const GameState& state) : GameStateView() {
	refresh(state);
}

void GameStateView::refresh(const GameState& state) {
	currentTurn = state.getCurrentTurn();
	maxTurn = state.getMaxTurn();
	activePlayerIndex = state.getActivePlayerIndex();
	hasRolledDice = state.getHasRolledDice();
	hasUsedSkillCard = state.getHasUsedSkillCard();

	activePlayerName.clear();
	if (const Player* activePlayer = state.getActivePlayer()) {
		activePlayerName = activePlayer->getUsername();
	}

	playerViews.clear();
	for (const Player* player : state.getPlayers()) {
		if (player == nullptr) {
			continue;
		}
		PlayerView view;
		view.username = player->getUsername();
		view.money = player->getMoney();
		view.position = player->getPosition();
		view.status = player->getStatus();
		view.propertyCount = static_cast<int>(player->getProperties().size());
		view.skillCardCount = static_cast<int>(player->getSkillCards().size());
		playerViews.push_back(view);
	}

	propertyViews.clear();
}

int GameStateView::getCurrentTurn() const {
	return currentTurn;
}

int GameStateView::getMaxTurn() const {
	return maxTurn;
}

const std::string& GameStateView::getActivePlayerName() const {
	return activePlayerName;
}

int GameStateView::getActivePlayerIndex() const {
	return activePlayerIndex;
}

const std::vector<PlayerView>& GameStateView::getPlayers() const {
	return playerViews;
}

const std::vector<PropertyView>& GameStateView::getProperties() const {
	return propertyViews;
}

bool GameStateView::getHasRolledDice() const {
	return hasRolledDice;
}

bool GameStateView::getHasUsedSkillCard() const {
	return hasUsedSkillCard;
}

std::string GameStateView::render(const GameState& state) const {
	GameStateView snapshot(state);
	std::string phase = "SETUP";
	if (state.getPhase() == GamePhase::RUNNING) {
		phase = "RUNNING";
	} else if (state.getPhase() == GamePhase::FINISHED) {
		phase = "FINISHED";
	}

	std::string result = "Phase: " + phase + " | Turn: " + std::to_string(snapshot.getCurrentTurn());
	if (!snapshot.getActivePlayerName().empty()) {
		result += " | Active: " + snapshot.getActivePlayerName();
	}
	return result;
}
