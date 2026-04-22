#ifndef MANAGER_HEADER_EVENT_BUS_HPP
#define MANAGER_HEADER_EVENT_BUS_HPP

#include <vector>

#include "models/event/header/IEventListener.hpp"

class EventBus {
public:
	void subscribe(IEventListener* listener);
	void unsubscribe(IEventListener* listener);
	void publish(const GameEvent& event) const;

private:
	std::vector<IEventListener*> listeners;

protected:
};

#endif
