#ifndef MANAGER_HEADER_BANKRUPTCY_MANAGER_HPP
#define MANAGER_HEADER_BANKRUPTCY_MANAGER_HPP

class BankruptcyManager {
public:
	BankruptcyManager();

	void setBankruptcyThreshold(int threshold);
	bool isBankrupt(int balance) const;

private:
	int bankruptcyThreshold;

protected:
};

#endif
