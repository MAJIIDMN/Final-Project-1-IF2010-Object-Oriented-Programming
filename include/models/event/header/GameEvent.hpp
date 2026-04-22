#ifndef MODELS_EVENT_HEADER_GAME_EVENT_HPP
#define MODELS_EVENT_HEADER_GAME_EVENT_HPP

#include <string>

#include "utils/Enums.hpp"

class GameEvent {
public:
	GameEvent(EventType type = EventType::TURN_STARTED, std::string payload = "");

	EventType getType() const;
	const std::string& getPayload() const;

private:
	EventType type;
	std::string payload;

protected:
};

#endif
