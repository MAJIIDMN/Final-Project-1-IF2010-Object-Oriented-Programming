#ifndef MANAGER_HEADER_AUCTION_MANAGER_HPP
#define MANAGER_HEADER_AUCTION_MANAGER_HPP

class AuctionManager {
public:
	AuctionManager();

	bool isActive() const;
	int getHighestBid() const;

	void startAuction(int openingBid = 0);
	bool placeBid(int bid);
	void closeAuction();

private:
	bool active;
	int highestBid;

protected:
};

#endif
