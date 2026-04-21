#ifndef BANK_HPP
#define BANK_HPP
#include <string>
#include "models/Money.hpp"
#include "models/Player.hpp"

class Bank {
public:
    void payPlayer(Player& player, const Money& amount, const std::string& reason);

    void collectFromPlayer(Player& player, const Money& amount, const std::string& reason);

    void transferBetweenPlayers(Player& from, Player& to, const Money& amount, const std::string& reason);
};

#endif
