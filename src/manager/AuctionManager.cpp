#include "manager/header/AuctionManager.hpp"

AuctionManager::AuctionManager() : active(false), highestBid(0) {}

bool AuctionManager::isActive() const {
	return active;
}

int AuctionManager::getHighestBid() const {
	return highestBid;
}

void AuctionManager::startAuction(int openingBid) {
	active = true;
	highestBid = openingBid;
}

bool AuctionManager::placeBid(int bid) {
	if (!active || bid <= highestBid) {
		return false;
	}
	highestBid = bid;
	return true;
}

void AuctionManager::closeAuction() {
	active = false;
}
