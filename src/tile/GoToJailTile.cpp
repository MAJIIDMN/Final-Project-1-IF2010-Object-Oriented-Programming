#include "tile/header/GoToJailTile.hpp"

#include "models/Player.hpp"
#include "models/board/header/Board.hpp"
#include "utils/Enums.hpp"
#include "utils/Types.hpp"
#include "core/log/header/TransactionLogger.hpp"

GoToJailTile::GoToJailTile(int id, const std::string& code, const std::string& name)
	: SpecialTile(id, code, name, TileType::GO_TO_JAIL) {}

void GoToJailTile::onLand(Player& player, GameContext& ctx, int /*diceTotal*/) {
	for (int i = 0; i < ctx.board.getSize(); ++i) {
		Tile* tile = ctx.board.getTile(i);
		if (tile && tile->getType() == TileType::JAIL) {
			player.setPosition(i);
			break;
		}
	}
	player.setStatus(PlayerStatus::JAILED);
	player.setConsecutiveDoubles(0);
	player.resetJailTurns();
	ctx.logger.log(ctx.currentTurn, player.getUsername(), "PENJARA", "Masuk penjara");
}
