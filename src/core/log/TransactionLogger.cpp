#include "core/log/header/TransactionLogger.hpp"

void TransactionLogger::log(const std::string& entry) {
	entries.push_back(entry);
}

const std::vector<std::string>& TransactionLogger::getEntries() const {
	return entries;
}

void TransactionLogger::clear() {
	entries.clear();
}
