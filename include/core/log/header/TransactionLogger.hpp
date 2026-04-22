#ifndef CORE_LOG_HEADER_TRANSACTION_LOGGER_HPP
#define CORE_LOG_HEADER_TRANSACTION_LOGGER_HPP

#include <string>
#include <vector>

#include "models/event/header/IEventListener.hpp"
#include "utils/Types.hpp"

class TransactionLogger : public IEventListener {
public:
	void onEvent(const GameEvent& event) override;

	void log(int turn, const std::string& username, const std::string& actionType, const std::string& detail);
	void logEntry(const LogEntry& entry);
	const std::vector<LogEntry>& getFullLog() const;
	std::vector<LogEntry> getLastN(int n) const;
	int getLogCount() const;
	std::vector<LogEntry> serializeForSave() const;
	void loadFromSave(const std::vector<LogEntry>& loadedEntries);

	void log(const std::string& entry);
	const std::vector<std::string>& getEntries() const;
	void clear();

private:
	static std::string eventTypeToString(EventType type);
	std::string formatEntry(const LogEntry& entry) const;

	std::vector<LogEntry> entries;
	mutable std::vector<std::string> legacyEntries;

protected:
};

#endif
