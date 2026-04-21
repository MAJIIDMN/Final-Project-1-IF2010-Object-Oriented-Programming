#include "core/FestivalManager.hpp"
#include "models/Player.hpp"
#include "models/effects/FestivalEffect.hpp"

// ── Private helper ────────────────────────────────────────────────────────────

int FestivalManager::multiplierFor(int timesApplied) {
    switch (timesApplied) {
        case 1:  return 2;
        case 2:  return 4;
        case 3:  return 8;
        default: return 1;
    }
}

// ── applyFestival ─────────────────────────────────────────────────────────────
//
// Rules (from plan.md):
//   - FestivalEffect on a property tracks timesApplied (max 3) and turnsRemaining.
//   - Multiplier sequence: 2× → 4× → 8× (one step per application).
//   - Each application lasts 3 turns.
//   - If timesApplied already equals 3, the festival cannot be applied again.
//   - The effect is also pushed onto the property-owner's Player effect list so
//     that Player::applyIncomingModifiers picks it up automatically.

FestivalResult FestivalManager::applyFestival(Player& player, PropertyTile& property) {
    auto it = activeEffects.find(&property);

    if (it != activeEffects.end()) {
        FestivalEffect& fe = it->second;

        if (fe.timesApplied >= 3) {
            // Maximum reached – cannot apply further.
            return FestivalResult{false, fe.multiplier, fe.turnsRemaining};
        }

        // Upgrade existing effect.
        ++fe.timesApplied;
        fe.multiplier      = multiplierFor(fe.timesApplied);
        fe.turnsRemaining  = 3;

        // Push a new Effect onto the player's active-effect list so
        // applyIncomingModifiers reflects the updated multiplier.
        player.addEffect(new FestivalRentEffect(fe.multiplier, 3));

        return FestivalResult{true, fe.multiplier, 3};
    }

    // First application.
    FestivalEffect fe;
    fe.owner          = &player;
    fe.timesApplied   = 1;
    fe.multiplier     = multiplierFor(1);
    fe.turnsRemaining = 3;
    activeEffects[&property] = fe;

    player.addEffect(new FestivalRentEffect(fe.multiplier, 3));

    return FestivalResult{true, fe.multiplier, 3};
}

// ── tickPlayerEffects ─────────────────────────────────────────────────────────

void FestivalManager::tickPlayerEffects(Player& player) {
    // Tick the player's own effect list (handles Shield, Discount, Festival…).
    player.tickEffects();

    // Also tick the FestivalManager-tracked effects for properties owned by
    // this player and remove any that have expired.
    for (auto it = activeEffects.begin(); it != activeEffects.end(); ) {
        if (it->second.owner == &player) {
            --it->second.turnsRemaining;
            if (it->second.turnsRemaining <= 0)
                it = activeEffects.erase(it);
            else
                ++it;
        } else {
            ++it;
        }
    }
}

// ── Query helpers ─────────────────────────────────────────────────────────────

int FestivalManager::getMultiplier(PropertyTile* property) const {
    auto it = activeEffects.find(property);
    return it != activeEffects.end() ? it->second.multiplier : 1;
}

int FestivalManager::getDuration(PropertyTile* property) const {
    auto it = activeEffects.find(property);
    return it != activeEffects.end() ? it->second.turnsRemaining : 0;
}

bool FestivalManager::hasActiveEffect(PropertyTile* property) const {
    return activeEffects.count(property) > 0;
}
