#ifndef MODELS_BOARD_HEADER_DICE_HPP
#define MODELS_BOARD_HEADER_DICE_HPP

#include <utility>
#include <random>

class Dice {
public:
	explicit Dice(int sides = 6);

	std::pair<int, int> rollPair();
	int roll();
	void setManual(int d1, int d2);

	int getDie1() const;
	int getDie2() const;
	int getTotal() const;
	bool isDouble() const;

	int getLastRoll() const;
	int getSides() const;

private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> distribution;
	int die1;
	int die2;
	bool manualSet;
	int sides;

protected:
};

#endif
