#ifndef MODELS_EVENT_HEADER_GAME_EVENT_HPP
#define MODELS_EVENT_HEADER_GAME_EVENT_HPP

#include <string>

#include "utils/Enums.hpp"

class GameEvent {
public:
	GameEvent(EventType type, int turn = 0, std::string username = "", std::string detail = "");
	virtual ~GameEvent();

	virtual EventType getType() const;
	int getTurn() const;
	const std::string& getUsername() const;
	const std::string& getDetail() const;
	const std::string& getPayload() const;

private:
	EventType type;
	int turn;
	std::string username;
	std::string detail;

protected:
};

class PlayerMovedEvent : public GameEvent {
public:
	PlayerMovedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class MoneyChangedEvent : public GameEvent {
public:
	MoneyChangedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class PropertyAcquiredEvent : public GameEvent {
public:
	PropertyAcquiredEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class RentPaidEvent : public GameEvent {
public:
	RentPaidEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class BuildingBuiltEvent : public GameEvent {
public:
	BuildingBuiltEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class MortgagedEvent : public GameEvent {
public:
	MortgagedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class RedeemedEvent : public GameEvent {
public:
	RedeemedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class FestivalActivatedEvent : public GameEvent {
public:
	FestivalActivatedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class AuctionStartedEvent : public GameEvent {
public:
	AuctionStartedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class BidPlacedEvent : public GameEvent {
public:
	BidPlacedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class CardDrawnEvent : public GameEvent {
public:
	CardDrawnEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class SkillCardUsedEvent : public GameEvent {
public:
	SkillCardUsedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class EffectAppliedEvent : public GameEvent {
public:
	EffectAppliedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class BankruptcyEvent : public GameEvent {
public:
	BankruptcyEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class TurnStartedEvent : public GameEvent {
public:
	TurnStartedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class TurnEndedEvent : public GameEvent {
public:
	TurnEndedEvent(int turn = 0, std::string username = "", std::string detail = "");
};

class GameOverEvent : public GameEvent {
public:
	GameOverEvent(int turn = 0, std::string username = "", std::string detail = "");
};

#endif
