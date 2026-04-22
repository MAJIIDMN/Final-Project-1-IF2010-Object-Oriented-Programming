#include "models/event/header/GameEvent.hpp"

GameEvent::GameEvent(EventType type, std::string payload) : type(type), payload(std::move(payload)) {}

EventType GameEvent::getType() const {
	return type;
}

const std::string& GameEvent::getPayload() const {
	return payload;
}
