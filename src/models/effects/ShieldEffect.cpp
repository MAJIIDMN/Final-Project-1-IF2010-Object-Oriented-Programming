#include "models/effects/ShieldEffect.hpp"

ShieldEffect::ShieldEffect(int turns) : Effect(turns) {}
bool ShieldEffect::blockPayment() const { 
    return true;
}

std::string ShieldEffect::getDescription() const { 
    return "Shield (blocks payments, " + std::to_string(remainingTurns) + " turn left)";
}

std::string ShieldEffect::getEffectType() const { 
    return "Shield";
}
