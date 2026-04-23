#include "models/event/header/GameEvent.hpp"

GameEvent::GameEvent(EventType type, int turn, std::string username, std::string detail)
	: type(type), turn(turn), username(std::move(username)), detail(std::move(detail)) {}

GameEvent::~GameEvent() = default;

EventType GameEvent::getType() const {
	return type;
}

int GameEvent::getTurn() const {
	return turn;
}

const std::string& GameEvent::getUsername() const {
	return username;
}

const std::string& GameEvent::getDetail() const {
	return detail;
}

const std::string& GameEvent::getPayload() const {
	return detail;
}

PlayerMovedEvent::PlayerMovedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::PLAYER_MOVED, turn, std::move(username), std::move(detail)) {}

MoneyChangedEvent::MoneyChangedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::MONEY_CHANGED, turn, std::move(username), std::move(detail)) {}

PropertyAcquiredEvent::PropertyAcquiredEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::PROPERTY_ACQUIRED, turn, std::move(username), std::move(detail)) {}

RentPaidEvent::RentPaidEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::RENT_PAID, turn, std::move(username), std::move(detail)) {}

BuildingBuiltEvent::BuildingBuiltEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::BUILDING_BUILT, turn, std::move(username), std::move(detail)) {}

MortgagedEvent::MortgagedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::MORTGAGED, turn, std::move(username), std::move(detail)) {}

RedeemedEvent::RedeemedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::REDEEMED, turn, std::move(username), std::move(detail)) {}

FestivalActivatedEvent::FestivalActivatedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::FESTIVAL_ACTIVATED, turn, std::move(username), std::move(detail)) {}

AuctionStartedEvent::AuctionStartedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::AUCTION_STARTED, turn, std::move(username), std::move(detail)) {}

BidPlacedEvent::BidPlacedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::BID_PLACED, turn, std::move(username), std::move(detail)) {}

CardDrawnEvent::CardDrawnEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::CARD_DRAWN, turn, std::move(username), std::move(detail)) {}

SkillCardUsedEvent::SkillCardUsedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::SKILL_CARD_USED, turn, std::move(username), std::move(detail)) {}

EffectAppliedEvent::EffectAppliedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::EFFECT_APPLIED, turn, std::move(username), std::move(detail)) {}

BankruptcyEvent::BankruptcyEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::BANKRUPTCY, turn, std::move(username), std::move(detail)) {}

TurnStartedEvent::TurnStartedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::TURN_STARTED, turn, std::move(username), std::move(detail)) {}

TurnEndedEvent::TurnEndedEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::TURN_ENDED, turn, std::move(username), std::move(detail)) {}

GameOverEvent::GameOverEvent(int turn, std::string username, std::string detail)
	: GameEvent(EventType::GAME_OVER, turn, std::move(username), std::move(detail)) {}
