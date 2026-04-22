#include "manager/header/BankruptcyManager.hpp"

BankruptcyManager::BankruptcyManager() : bankruptcyThreshold(0) {}

void BankruptcyManager::setBankruptcyThreshold(int threshold) {
	bankruptcyThreshold = threshold;
}

bool BankruptcyManager::isBankrupt(int balance) const {
	return balance <= bankruptcyThreshold;
}
