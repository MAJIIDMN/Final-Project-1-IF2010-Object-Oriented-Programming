#ifndef MANAGER_HEADER_EVENT_BUS_HPP
#define MANAGER_HEADER_EVENT_BUS_HPP

#include <map>
#include <vector>

#include "models/event/header/IEventListener.hpp"

class EventBus {
public:
	void subscribe(EventType type, IEventListener* listener);
	void subscribe(IEventListener* listener);
	void unsubscribe(EventType type, IEventListener* listener);
	void unsubscribe(IEventListener* listener);
	void publish(const GameEvent& event) const;

private:
	std::map<EventType, std::vector<IEventListener*>> listeners;

protected:
};

#endif
