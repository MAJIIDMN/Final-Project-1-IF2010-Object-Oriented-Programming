#include "manager/header/EventBus.hpp"

#include <algorithm>

void EventBus::subscribe(IEventListener* listener) {
	if (listener == nullptr) {
		return;
	}
	listeners.push_back(listener);
}

void EventBus::unsubscribe(IEventListener* listener) {
	listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}

void EventBus::publish(const GameEvent& event) const {
	for (IEventListener* listener : listeners) {
		if (listener != nullptr) {
			listener->onEvent(event);
		}
	}
}
