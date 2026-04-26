#include "manager/header/BankruptcyManager.hpp"

#include <algorithm>
#include <iostream>

#include "manager/header/AuctionManager.hpp"
#include "manager/header/EventBus.hpp"
#include "manager/header/TurnManager.hpp"
#include "models/Player.hpp"
#include "models/board/header/Board.hpp"
#include "models/board/header/ColorGroup.hpp"
#include "models/event/header/GameEvent.hpp"
#include "tile/header/PropertyTile.hpp"
#include "tile/header/StreetTile.hpp"
#include "core/Bank.hpp"
#include "core/FestivalManager.hpp"
#include "core/log/header/TransactionLogger.hpp"
#include "core/state/header/GameState.hpp"
#include "controllers/PlayerController.hpp"
#include "utils/GameUtils.hpp"

BankruptcyManager::BankruptcyManager() : bankruptcyThreshold(0) {}

void BankruptcyManager::setBankruptcyThreshold(int threshold) {
	bankruptcyThreshold = threshold;
}

bool BankruptcyManager::isBankrupt(int balance) const {
	return balance <= bankruptcyThreshold;
}

LiquidationState BankruptcyManager::buildLiquidationStateSnapshot(const Player& debtor, const Board& board, Money obligation) {
	LiquidationState state;
	state.obligation = obligation;
	state.currentBalance = debtor.getMoney();
	state.maxLiquidation = debtor.getMoney();

	int optionIndex = 1;
	for (PropertyTile* property : debtor.getProperties()) {
		if (!property) {
			continue;
		}

		if (!property->isMortgaged()) {
			const Money sellValue = property->getSaleValue();
			if (sellValue.isPositive()) {
				LiquidationOption option;
				option.index = optionIndex++;
				option.type = LiquidationType::SELL;
				option.code = property->getCode();
				option.name = property->getName();
				option.value = sellValue;
				option.description = "Jual ke Bank";
				state.options.push_back(option);
				state.maxLiquidation += sellValue;
			}
		}

		if (property->canMortgageNow(board)) {
			LiquidationOption option;
			option.index = optionIndex++;
			option.type = LiquidationType::MORTGAGE;
			option.code = property->getCode();
			option.name = property->getName();
			option.value = property->getMortgageValue();
			option.description = "Gadai";
			state.options.push_back(option);
		}
	}

	return state;
}

void BankruptcyManager::execute(Player& debtor, Player* creditor, Money obligation,
	GameState& gameState, Board& board, Bank& bank,
	FestivalManager& festivalManager, TransactionLogger& logger,
	TurnManager& turnManager, AuctionManager& auctionManager,
	EventBus& eventBus) {
	if (debtor.isBankrupt()) {
		return;
	}

	auto settleObligation = [&]() {
		if (creditor && !creditor->isBankrupt() && creditor != &debtor) {
			bank.transferBetweenPlayers(debtor, *creditor, obligation, "Pelunasan likuidasi");
			logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "LIKUIDASI",
				"Lunas ke " + creditor->getUsername() + " " + obligation.toString());
		} else {
			bank.collectFromPlayer(debtor, obligation, "Pelunasan likuidasi");
			logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "LIKUIDASI",
				"Lunas ke Bank " + obligation.toString());
		}
	};

	auto applyLiquidationOption = [&](const LiquidationOption& option) -> bool {
		PropertyTile* property = board.getPropertyByCode(normalizeCode(option.code));
		if (!property || property->getOwner() != &debtor) {
			return false;
		}

		if (option.type == LiquidationType::SELL) {
			if (property->isMortgaged()) {
				return false;
			}

			const Money value = property->getSaleValue();
			festivalManager.removeEffectsForProperty(property);
			if (auto* street = dynamic_cast<StreetTile*>(property)) {
				while (street->getBuildingLevel() > 0) {
					street->demolish();
				}
			}
			debtor.removeProperty(property);
			property->setOwner(nullptr);
			property->setStatus(PropertyStatus::BANK);
			bank.payPlayer(debtor, value, "Likuidasi jual ke bank");
			board.updateMonopolies();
			logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "LIKUIDASI",
				"Jual " + property->getLabel() + " ke Bank " + value.toString());
			return true;
		}

		if (option.type == LiquidationType::MORTGAGE) {
			if (!property->canMortgageNow(board)) {
				return false;
			}

			property->setStatus(PropertyStatus::MORTGAGED);
			bank.payPlayer(debtor, property->getMortgageValue(), "Likuidasi gadai");
			board.updateMonopolies();
			logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "LIKUIDASI",
				"Gadai " + property->getLabel() + " " + property->getMortgageValue().toString());
			return true;
		}

		return false;
	};

	if (obligation.isPositive()) {
		const Money effectiveObligation = debtor.applyOutgoingModifiers(obligation);
		if (!effectiveObligation.isPositive()) {
			logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "LIKUIDASI",
				"Kewajiban " + obligation.toString() + " terblokir modifier pembayaran");
			return;
		}

		if (debtor.canAfford(effectiveObligation)) {
			settleObligation();
			return;
		}

		LiquidationState liquidationState = buildLiquidationStateSnapshot(debtor, board, obligation);
		if (liquidationState.maxLiquidation >= effectiveObligation) {
			logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "LIKUIDASI",
				"Wajib likuidasi untuk menutup kewajiban " + obligation.toString());
			while (!debtor.canAfford(effectiveObligation)) {
				liquidationState = buildLiquidationStateSnapshot(debtor, board, obligation);
				if (liquidationState.options.empty()) {
					break;
				}

				int choice = 0;
				if (PlayerController* controller = debtor.getController()) {
					choice = controller->decideLiquidation(liquidationState);
				}

				const LiquidationOption* selected = nullptr;
				for (const LiquidationOption& option : liquidationState.options) {
					if (option.index == choice) {
						selected = &option;
						break;
					}
				}
				if (!selected) {
					selected = &*std::max_element(
						liquidationState.options.begin(),
						liquidationState.options.end(),
						[](const LiquidationOption& a, const LiquidationOption& b) {
							return a.value < b.value;
						});
				}
				if (!selected || !applyLiquidationOption(*selected)) {
					break;
				}
			}

			if (debtor.canAfford(effectiveObligation)) {
				settleObligation();
				return;
			}
		}
	}

	debtor.setStatus(PlayerStatus::BANKRUPT);
	eventBus.publish(BankruptcyEvent(gameState.getCurrentTurn(), debtor.getUsername(), "Bangkrut"));

	const std::vector<PropertyTile*> props(debtor.getProperties());
	logger.log(gameState.getCurrentTurn(), debtor.getUsername(), "BANGKRUT",
		creditor && !creditor->isBankrupt() && creditor != &debtor
			? ("Bangkrut ke " + creditor->getUsername())
			: "Bangkrut ke Bank");

	if (creditor && !creditor->isBankrupt() && creditor != &debtor) {
		const Money remainingCash = debtor.getMoney();
		if (remainingCash.isPositive()) {
			bank.transferBetweenPlayers(debtor, *creditor, remainingCash, "Aset bangkrut");
		}

		for (PropertyTile* prop : props) {
			if (!prop) {
				continue;
			}
			festivalManager.removeEffectsForProperty(prop);
			debtor.removeProperty(prop);
			prop->setOwner(creditor);
			creditor->addProperty(prop);
		}
	} else {
		const Money remainingCash = debtor.getMoney();
		if (remainingCash.isPositive()) {
			debtor.deductMoney(remainingCash);
		}

		for (PropertyTile* prop : props) {
			if (!prop) {
				continue;
			}
			festivalManager.removeEffectsForProperty(prop);
			if (auto* street = dynamic_cast<StreetTile*>(prop)) {
				while (street->getBuildingLevel() > 0) {
					street->demolish();
				}
			}
			debtor.removeProperty(prop);
			prop->setOwner(nullptr);
			prop->setStatus(PropertyStatus::BANK);
		}

		board.updateMonopolies();
		for (PropertyTile* prop : props) {
			if (prop) {
				auctionManager.runAuction(*prop, debtor, gameState.getPlayers(), bank, board, logger, gameState.getCurrentTurn());
			}
		}
	}

	board.updateMonopolies();

	std::vector<Player*> order(gameState.getTurnOrder().begin(), gameState.getTurnOrder().end());
	int removedIdx = -1;
	for (std::size_t i = 0; i < order.size(); ++i) {
		if (order[i] == &debtor) {
			removedIdx = static_cast<int>(i);
			order.erase(order.begin() + static_cast<std::ptrdiff_t>(i));
			break;
		}
	}
	if (removedIdx >= 0) {
		turnManager.removePlayer(removedIdx);
		gameState.setTurnOrder(order);
		gameState.setActivePlayerIndex(turnManager.getCurrentPlayerIndex());
	}
}
