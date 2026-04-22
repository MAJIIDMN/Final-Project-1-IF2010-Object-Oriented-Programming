#ifndef CORE_COMMAND_HEADER_COMMAND_HPP
#define CORE_COMMAND_HEADER_COMMAND_HPP

#include <string>

class GameEngine;
class Player;
class GameStateView;

class Command {
public:
	explicit Command(std::string name = "", std::string help = "");
	virtual ~Command();

	virtual void execute(GameEngine& engine, Player& player) = 0;
	virtual bool canExecute(const GameStateView& state) const;

	const std::string& getName() const;
	const std::string& getHelp() const;

private:
	std::string name;
	std::string help;

protected:
};

#endif