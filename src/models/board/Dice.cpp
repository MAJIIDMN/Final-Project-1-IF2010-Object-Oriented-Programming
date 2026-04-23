#include "models/board/header/Dice.hpp"

#include <algorithm>

Dice::Dice(int sides)
	: rng(std::random_device{}()), distribution(1, sides > 1 ? sides : 6), die1(1), die2(1), manualSet(false),
	  sides(sides > 1 ? sides : 6) {}

std::pair<int, int> Dice::rollPair() {
	if (!manualSet) {
		die1 = distribution(rng);
		die2 = distribution(rng);
	} else {
		manualSet = false;
	}
	return std::make_pair(die1, die2);
}

int Dice::roll() {
	rollPair();
	return getTotal();
}

void Dice::setManual(int d1, int d2) {
	die1 = std::max(1, std::min(d1, sides));
	die2 = std::max(1, std::min(d2, sides));
	manualSet = true;
}

int Dice::getDie1() const {
	return die1;
}

int Dice::getDie2() const {
	return die2;
}

int Dice::getTotal() const {
	return die1 + die2;
}

bool Dice::isDouble() const {
	return die1 == die2;
}

int Dice::getLastRoll() const {
	return getTotal();
}

int Dice::getSides() const {
	return sides;
}
