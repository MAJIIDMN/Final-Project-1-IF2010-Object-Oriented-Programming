#ifndef MODELS_BOARD_HEADER_DICE_HPP
#define MODELS_BOARD_HEADER_DICE_HPP

#include <random>

class Dice {
public:
	explicit Dice(int sides = 6);

	int roll();
	int getLastRoll() const;
	int getSides() const;

private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> distribution;
	int lastRoll;
	int sides;

protected:
};

#endif
