#ifndef CORE_COMMAND_HEADER_COMMAND_BASIC_COMMANDS_HPP
#define CORE_COMMAND_HEADER_COMMAND_BASIC_COMMANDS_HPP

#include "core/command/header/Command.hpp"

class RollDiceCommand : public Command {
public:
	RollDiceCommand();

	void execute(GameEngine& engine, Player& player) override;
	bool canExecute(const GameStateView& state) const override;
};

class EndTurnCommand : public Command {
public:
	EndTurnCommand();

	void execute(GameEngine& engine, Player& player) override;
	bool canExecute(const GameStateView& state) const override;
};

class PrintBoardCommand : public Command {
public:
	PrintBoardCommand();

	void execute(GameEngine& engine, Player& player) override;
};

class PrintLogCommand : public Command {
public:
	PrintLogCommand();

	void execute(GameEngine& engine, Player& player) override;
};

#endif
