#include "core/Bank.hpp"
#include "models/Player.hpp"
#include "utils/Exceptions.hpp"

void Bank::payPlayer(Player& player, const Money& amount, const std::string& /*reason*/) {
    player.addMoney(amount);
}

void Bank::collectFromPlayer(Player& player, const Money& amount, const std::string& /*reason*/) {
    if (!player.canAfford(amount))
        throw InsufficientFundsException(amount, player.getMoney());
    player.deductMoney(amount);
}

void Bank::transferBetweenPlayers(Player& from, Player& to, const Money& amount, const std::string& /*reason*/) {
    if (!from.canAfford(amount))
        throw InsufficientFundsException(amount, from.getMoney());
    from.deductMoney(amount);
    to.addMoney(amount);
}
