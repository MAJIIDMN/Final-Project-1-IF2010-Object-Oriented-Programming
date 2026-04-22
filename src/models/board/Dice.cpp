#include "models/board/header/Dice.hpp"

Dice::Dice(int sides) : rng(std::random_device{}()), distribution(1, sides), lastRoll(1), sides(sides) {}

int Dice::roll() {
	lastRoll = distribution(rng);
	return lastRoll;
}

int Dice::getLastRoll() const {
	return lastRoll;
}

int Dice::getSides() const {
	return sides;
}
