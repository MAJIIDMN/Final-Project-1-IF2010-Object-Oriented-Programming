#ifndef CORE_LOG_HEADER_TRANSACTION_LOGGER_HPP
#define CORE_LOG_HEADER_TRANSACTION_LOGGER_HPP

#include <string>
#include <vector>

class TransactionLogger {
public:
	void log(const std::string& entry);
	const std::vector<std::string>& getEntries() const;
	void clear();

private:
	std::vector<std::string> entries;

protected:
};

#endif
