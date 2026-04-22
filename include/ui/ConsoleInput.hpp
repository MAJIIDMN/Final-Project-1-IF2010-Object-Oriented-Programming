#pragma once

#include "ui/IGameInput.hpp"

class ConsoleInput : public IGameInput {
public:
    int getMenuChoice(const vector<string>& options) override;
    string getCommand() override;
    bool getYesNo(const string& prompt) override;
    int getNumberInRange(const string& prompt, int min, int max) override;
    string getString(const string& prompt) override;
    pair<int, int> getManualDice() override;
    AuctionAction getAuctionAction(const string& playerName, int currentBid, int playerMoney) override;
    TaxChoice getTaxChoice() override;
    int getLiquidationChoice(int numOptions) override;
    int getSkillCardChoice(int numCards) override;
    string getPropertyCodeInput(const string& prompt) override;
};
