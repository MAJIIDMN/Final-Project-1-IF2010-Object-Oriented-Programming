#pragma once
#include <string>
#include <vector>

enum class AppScreen {
    LANDING,
    NEW_GAME_NUM_PLAYERS,
    NEW_GAME_CUST_PLAYER,
    NEW_GAME_CUST_MAP,
    LOAD_GAME,
    IN_GAME,
    GAME_OVER,
};

struct SetupState {
    int  numPlayers{2};
    int  numTiles{40};
    std::vector<std::string> playerNames;
    std::vector<bool>        isComputer;
    std::vector<int>         playerColors;  // 0-3 → cyan/pink/yellow/green
    int  startingMoney{1500};
    std::string loadFilePath;
};
