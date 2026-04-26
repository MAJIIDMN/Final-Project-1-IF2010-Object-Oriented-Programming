#include "tile/header/FestivalTile.hpp"

#include "models/Player.hpp"
#include "controllers/PlayerController.hpp"
#include "utils/Types.hpp"
#include "utils/GameUtils.hpp"
#include "tile/header/PropertyTile.hpp"
#include "core/FestivalManager.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "models/board/header/Board.hpp"

FestivalTile::FestivalTile(int id, const std::string& code, const std::string& name)
	: ActionTile(id, code, name, TileType::FESTIVAL) {}

void FestivalTile::onLand(Player& player, GameContext& ctx, int /*diceTotal*/) {
	std::vector<PropertyInfo> owned;
	for (PropertyTile* property : player.getProperties()) {
		if (property && !property->isMortgaged()) {
			owned.push_back(property->toInfo());
		}
	}
	if (owned.empty()) {
		std::cout << "Tidak ada properti untuk festival.\n";
		return;
	}

	std::string code;
	if (PlayerController* controller = player.getController()) {
		code = controller->decideFestivalProperty(owned);
	}
	PropertyTile* property = ctx.board.getPropertyByCode(normalizeCode(code));
	if (!property || property->getOwner() != &player) {
		ctx.logger.log(ctx.currentTurn, player.getUsername(), "FESTIVAL",
			"Pilihan properti tidak valid. Pilih properti yang dimiliki.");
		return;
	}
	FestivalResult result = ctx.festivalManager.applyFestival(player, *property);
	ctx.logger.log(ctx.currentTurn, player.getUsername(), "FESTIVAL",
		"Aktifkan festival di " + property->getLabel() + " x" + std::to_string(result.getMultiplier()));
}
