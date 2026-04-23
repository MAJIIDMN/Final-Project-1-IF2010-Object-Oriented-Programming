#include "manager/header/EventBus.hpp"

#include <algorithm>

void EventBus::subscribe(EventType type, IEventListener* listener) {
	if (listener == nullptr) {
		return;
	}

	auto& bucket = listeners[type];
	if (std::find(bucket.begin(), bucket.end(), listener) == bucket.end()) {
		bucket.push_back(listener);
	}
}

void EventBus::subscribe(IEventListener* listener) {
	if (listener == nullptr) {
		return;
	}

	subscribe(EventType::PLAYER_MOVED, listener);
	subscribe(EventType::MONEY_CHANGED, listener);
	subscribe(EventType::PROPERTY_ACQUIRED, listener);
	subscribe(EventType::RENT_PAID, listener);
	subscribe(EventType::BUILDING_BUILT, listener);
	subscribe(EventType::MORTGAGED, listener);
	subscribe(EventType::REDEEMED, listener);
	subscribe(EventType::FESTIVAL_ACTIVATED, listener);
	subscribe(EventType::AUCTION_STARTED, listener);
	subscribe(EventType::BID_PLACED, listener);
	subscribe(EventType::CARD_DRAWN, listener);
	subscribe(EventType::SKILL_CARD_USED, listener);
	subscribe(EventType::EFFECT_APPLIED, listener);
	subscribe(EventType::BANKRUPTCY, listener);
	subscribe(EventType::TURN_STARTED, listener);
	subscribe(EventType::TURN_ENDED, listener);
	subscribe(EventType::GAME_OVER, listener);
}

void EventBus::unsubscribe(EventType type, IEventListener* listener) {
	auto it = listeners.find(type);
	if (it == listeners.end()) {
		return;
	}

	auto& bucket = it->second;
	bucket.erase(std::remove(bucket.begin(), bucket.end(), listener), bucket.end());
}

void EventBus::unsubscribe(IEventListener* listener) {
	for (auto& entry : listeners) {
		auto& bucket = entry.second;
		bucket.erase(std::remove(bucket.begin(), bucket.end(), listener), bucket.end());
	}
}

void EventBus::publish(const GameEvent& event) const {
	auto it = listeners.find(event.getType());
	if (it == listeners.end()) {
		return;
	}

	for (IEventListener* listener : it->second) {
		if (listener != nullptr) {
			listener->onEvent(event);
		}
	}
}
