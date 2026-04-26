#pragma once

#include <string>
#include <utility>
#include <vector>

#include "ui/UiDtos.hpp"
#include "utils/Enums.hpp"
#include "utils/Types.hpp"

class IGameInput {
public:
    virtual ~IGameInput() = default;

    virtual int    getPlayerCount() = 0;
    virtual string getPlayerName(int playerIdx) = 0;

    virtual string getCommand() = 0;
    virtual int    getMenuChoice(const vector<string>& options) = 0;
    virtual bool   getYesNo(const string& prompt) = 0;
    virtual int    getNumberInRange(const string& prompt, int min, int max) = 0;
    virtual string getString(const string& prompt) = 0;

    virtual pair<int, int> getManualDice() = 0;

    virtual AuctionDecision getAuctionDecision(
        const string& playerName, int currentBid, int playerMoney) = 0;

    virtual TaxChoice getTaxChoice() = 0;
    virtual int       getLiquidationChoice(const LiquidationState& liquidationState) = 0;
    virtual int       getSkillCardChoice(const vector<CardInfo>& cards) = 0;
    virtual string    getPropertyCodeInput(const string& prompt) = 0;
};
