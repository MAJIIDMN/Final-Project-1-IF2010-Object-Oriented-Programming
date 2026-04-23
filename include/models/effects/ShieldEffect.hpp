#ifndef SHIELDEFFECT_HPP
#define SHIELDEFFECT_HPP
#include "models/effects/Effect.hpp"

class ShieldEffect : public Effect {
public:
    explicit ShieldEffect(int turns = 1);
    bool blockPayment() const override;
    std::string getDescription() const override;
    std::string getEffectType() const override;
};

#endif
