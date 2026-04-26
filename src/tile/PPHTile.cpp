#include "tile/header/PPHTile.hpp"

#include "models/Player.hpp"
#include "controllers/PlayerController.hpp"
#include "utils/Types.hpp"
#include "utils/Enums.hpp"
#include "core/Bank.hpp"
#include "core/config/header/GameConfig.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "manager/header/BankruptcyManager.hpp"

PPHTile::PPHTile(int id, const std::string& code, const std::string& name,
				 int flatAmount, int percentage)
	: TaxTile(id, code, name, TileType::TAX_PPH),
	  flatAmount(flatAmount), percentage(percentage) {}

Money PPHTile::calculateTax(const Player& /*player*/) const {
	return Money::zero();
}

int PPHTile::getFlatAmount() const {
	return flatAmount;
}

int PPHTile::getPercentage() const {
	return percentage;
}

void PPHTile::onLand(Player& player, GameContext& ctx, int /*diceTotal*/) {
	TaxChoice choice = TaxChoice::FLAT;
	if (PlayerController* controller = player.getController()) {
		choice = controller->decideTax(ctx.config.getPphFlat(), 0);
	}
	Money tax(0);
	if (choice == TaxChoice::PERCENTAGE) {
		const int percentAmount = player.getTotalWealth().getAmount() * ctx.config.getPphPercentage() / 100;
		tax = Money(percentAmount);
	} else {
		tax = Money(ctx.config.getPphFlat());
	}

	if (!player.canAfford(player.applyOutgoingModifiers(tax))) {
		ctx.bankruptcyManager->execute(player, nullptr, tax, *ctx.gameState, ctx.board, ctx.bank,
			ctx.festivalManager, ctx.logger, *ctx.turnManager, *ctx.auctionManager, *ctx.eventBus);
		return;
	}
	ctx.bank.collectFromPlayer(player, tax, "Pajak");
	std::cout << player.getUsername() << " membayar pajak penghasilan " << tax.toString() << ".\n";
	ctx.logger.log(ctx.currentTurn, player.getUsername(), "PAJAK",
		"Bayar pajak penghasilan " + tax.toString());
}
