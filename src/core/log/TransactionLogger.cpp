#include "core/log/header/TransactionLogger.hpp"

#include <algorithm>

void TransactionLogger::onEvent(const GameEvent& event) {
	const std::string username = event.getUsername().empty() ? "SYSTEM" : event.getUsername();
	log(event.getTurn(), username, eventTypeToString(event.getType()), event.getDetail());
}

void TransactionLogger::log(int turn, const std::string& username, const std::string& actionType,
	const std::string& detail) {
	LogEntry entry;
	entry.turn = turn;
	entry.username = username;
	entry.actionType = actionType;
	entry.detail = detail;
	logEntry(entry);
}

void TransactionLogger::logEntry(const LogEntry& entry) {
	entries.push_back(entry);
	legacyEntries.push_back(formatEntry(entry));
}

const std::vector<LogEntry>& TransactionLogger::getFullLog() const {
	return entries;
}

std::vector<LogEntry> TransactionLogger::getLastN(int n) const {
	if (n <= 0 || entries.empty()) {
		return {};
	}

	const int safeN = std::min(n, static_cast<int>(entries.size()));
	const auto beginIt = entries.end() - safeN;
	return std::vector<LogEntry>(beginIt, entries.end());
}

int TransactionLogger::getLogCount() const {
	return static_cast<int>(entries.size());
}

std::vector<LogEntry> TransactionLogger::serializeForSave() const {
	return entries;
}

void TransactionLogger::loadFromSave(const std::vector<LogEntry>& loadedEntries) {
	entries = loadedEntries;
	legacyEntries.clear();
	legacyEntries.reserve(entries.size());
	for (const LogEntry& entry : entries) {
		legacyEntries.push_back(formatEntry(entry));
	}
}

void TransactionLogger::log(const std::string& entry) {
	LogEntry structured;
	structured.turn = 0;
	structured.username = "SYSTEM";
	structured.actionType = "LEGACY";
	structured.detail = entry;
	logEntry(structured);
}

const std::vector<std::string>& TransactionLogger::getEntries() const {
	if (legacyEntries.size() != entries.size()) {
		legacyEntries.clear();
		legacyEntries.reserve(entries.size());
		for (const LogEntry& entry : entries) {
			legacyEntries.push_back(formatEntry(entry));
		}
	}
	return legacyEntries;
}

void TransactionLogger::clear() {
	entries.clear();
	legacyEntries.clear();
}

std::string TransactionLogger::eventTypeToString(EventType type) {
	switch (type) {
	case EventType::PLAYER_MOVED:
		return "PLAYER_MOVED";
	case EventType::MONEY_CHANGED:
		return "MONEY_CHANGED";
	case EventType::PROPERTY_ACQUIRED:
		return "PROPERTY_ACQUIRED";
	case EventType::RENT_PAID:
		return "RENT_PAID";
	case EventType::BUILDING_BUILT:
		return "BUILDING_BUILT";
	case EventType::MORTGAGED:
		return "MORTGAGED";
	case EventType::REDEEMED:
		return "REDEEMED";
	case EventType::FESTIVAL_ACTIVATED:
		return "FESTIVAL_ACTIVATED";
	case EventType::AUCTION_STARTED:
		return "AUCTION_STARTED";
	case EventType::BID_PLACED:
		return "BID_PLACED";
	case EventType::CARD_DRAWN:
		return "CARD_DRAWN";
	case EventType::SKILL_CARD_USED:
		return "SKILL_CARD_USED";
	case EventType::EFFECT_APPLIED:
		return "EFFECT_APPLIED";
	case EventType::BANKRUPTCY:
		return "BANKRUPTCY";
	case EventType::TURN_STARTED:
		return "TURN_STARTED";
	case EventType::TURN_ENDED:
		return "TURN_ENDED";
	case EventType::GAME_OVER:
		return "GAME_OVER";
	default:
		return "UNKNOWN";
	}
}

std::string TransactionLogger::formatEntry(const LogEntry& entry) const {
	return "[T" + std::to_string(entry.turn) + "] " + entry.username + " | " + entry.actionType + " | " +
		entry.detail;
}
