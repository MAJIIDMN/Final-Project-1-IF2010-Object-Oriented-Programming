#include "tile/header/ChanceTile.hpp"

#include "models/Player.hpp"
#include "models/cards/CardSystem.hpp"
#include "models/cards/CardResult.hpp"
#include "utils/Types.hpp"
#include "core/Bank.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "manager/header/BankruptcyManager.hpp"
#include "models/board/header/Board.hpp"

ChanceTile::ChanceTile(int id, const std::string& code, const std::string& name)
	: ActionTile(id, code, name, TileType::CHANCE) {}

void ChanceTile::onLand(Player& player, GameContext& ctx, int diceTotal) {
	if (!ctx.cardSystem) return;
	auto card = ctx.cardSystem->drawChance();
	if (!card) return;
	std::cout << "Kartu Kesempatan: " << card->getDescription() << "\n";
	ctx.logger.log(ctx.currentTurn, player.getUsername(), "KARTU",
		"Kesempatan: " + card->getDescription());

	CardResult rawResult = card->execute(player, ctx);

	const auto resolvePeerPaymentCard = [&](const CardResult& rawResult) -> bool {
		if (rawResult.action != CardResultAction::RECEIVE_FROM_EACH_PLAYER &&
			rawResult.action != CardResultAction::PAY_EACH_PLAYER) {
			return false;
		}

		for (Player* other : ctx.players) {
			if (!other || other == &player || other->isBankrupt()) {
				continue;
			}

			if (rawResult.action == CardResultAction::RECEIVE_FROM_EACH_PLAYER) {
				if (!other->canAfford(other->applyOutgoingModifiers(rawResult.amount))) {
					ctx.bankruptcyManager->execute(*other, &player, rawResult.amount, *ctx.gameState, ctx.board, ctx.bank,
						ctx.festivalManager, ctx.logger, *ctx.turnManager, *ctx.auctionManager, *ctx.eventBus);
				} else {
					ctx.bank.transferBetweenPlayers(*other, player, rawResult.amount, rawResult.message);
				}
				continue;
			}

			if (player.isBankrupt()) {
				break;
			}
			if (!player.canAfford(player.applyOutgoingModifiers(rawResult.amount))) {
				ctx.bankruptcyManager->execute(player, other, rawResult.amount, *ctx.gameState, ctx.board, ctx.bank,
					ctx.festivalManager, ctx.logger, *ctx.turnManager, *ctx.auctionManager, *ctx.eventBus);
			} else if (!player.isBankrupt()) {
				ctx.bank.transferBetweenPlayers(player, *other, rawResult.amount, rawResult.message);
			}
		}

		if (!rawResult.message.empty()) {
			ctx.logger.log(ctx.currentTurn, player.getUsername(), "KARTU", rawResult.message);
		}
		return true;
	};

	if (resolvePeerPaymentCard(rawResult)) {
		ctx.cardSystem->discardChance(std::move(card));
		return;
	}

	CardResult result = CardSystem::applyImmediateResult(player, ctx, rawResult);
	ctx.cardSystem->discardChance(std::move(card));

	if (!result.success) {
		std::cout << result.message << "\n";
		ctx.logger.log(ctx.currentTurn, player.getUsername(), "KARTU", result.message);
		if (rawResult.action == CardResultAction::PAY_BANK) {
			ctx.bankruptcyManager->execute(player, nullptr, rawResult.amount, *ctx.gameState, ctx.board, ctx.bank,
				ctx.festivalManager, ctx.logger, *ctx.turnManager, *ctx.auctionManager, *ctx.eventBus);
		} else if (rawResult.action == CardResultAction::PAY_EACH_PLAYER) {
			ctx.bankruptcyManager->execute(player, nullptr,
				Money(rawResult.amount.getAmount() * Player::countActiveOthers(ctx.players, player)),
				*ctx.gameState, ctx.board, ctx.bank,
				ctx.festivalManager, ctx.logger, *ctx.turnManager, *ctx.auctionManager, *ctx.eventBus);
		}
		return;
	}

	if (!result.message.empty()) {
		ctx.logger.log(ctx.currentTurn, player.getUsername(), "KARTU", result.message);
	}
	if (result.success && result.resolveLanding) {
		Tile* newTile = ctx.board.getTile(player.getPosition());
		if (newTile && !player.isBankrupt()) {
			std::cout << "Mendarat di " << newTile->getName() << " (" << newTile->getCode() << ").\n";
			ctx.logger.log(ctx.currentTurn, player.getUsername(), "PETAK",
				"Mendarat di " + newTile->getLabel());
			newTile->onLand(player, ctx, diceTotal);
		}
	}
}
