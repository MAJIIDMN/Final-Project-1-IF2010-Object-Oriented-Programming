#ifndef MODELS_EVENT_HEADER_IEVENT_LISTENER_HPP
#define MODELS_EVENT_HEADER_IEVENT_LISTENER_HPP

#include "models/event/header/GameEvent.hpp"

class IEventListener {
public:
	virtual ~IEventListener();
	virtual void onEvent(const GameEvent& event) = 0;

private:

protected:
};

#endif
