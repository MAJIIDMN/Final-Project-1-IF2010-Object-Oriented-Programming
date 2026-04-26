#include "tile/header/PBMTile.hpp"

#include "models/Player.hpp"
#include "utils/Types.hpp"
#include "core/Bank.hpp"
#include "core/config/header/GameConfig.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "manager/header/BankruptcyManager.hpp"

PBMTile::PBMTile(int id, const std::string& code, const std::string& name, int flatAmount)
	: TaxTile(id, code, name, TileType::TAX_PBM), flatAmount(flatAmount) {}

Money PBMTile::calculateTax(const Player& /*player*/) const {
	return Money(flatAmount);
}

int PBMTile::getFlatAmount() const {
	return flatAmount;
}

void PBMTile::onLand(Player& player, GameContext& ctx, int /*diceTotal*/) {
	Money tax(ctx.config.getPbmFlat());
	if (!player.canAfford(player.applyOutgoingModifiers(tax))) {
		ctx.bankruptcyManager->execute(player, nullptr, tax, *ctx.gameState, ctx.board, ctx.bank,
			ctx.festivalManager, ctx.logger, *ctx.turnManager, *ctx.auctionManager, *ctx.eventBus);
		return;
	}
	ctx.bank.collectFromPlayer(player, tax, "Pajak");
	std::cout << player.getUsername() << " membayar pajak barang mewah " << tax.toString() << ".\n";
	ctx.logger.log(ctx.currentTurn, player.getUsername(), "PAJAK",
		"Bayar pajak barang mewah " + tax.toString());
}
