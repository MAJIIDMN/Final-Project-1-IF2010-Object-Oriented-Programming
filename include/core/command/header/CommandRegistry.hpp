#ifndef CORE_COMMAND_HEADER_COMMAND_REGISTRY_HPP
#define CORE_COMMAND_HEADER_COMMAND_REGISTRY_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/command/header/Command.hpp"

class GameEngine;
class Player;
class GameStateView;

class CommandRegistry {
public:
	CommandRegistry();

	void registerCommand(std::unique_ptr<Command> command);
	bool hasCommand(const std::string& name) const;
	bool execute(const std::string& name, GameEngine& engine, Player& player) const;
	bool parseAndExecute(const std::string& input, GameEngine& engine, Player& player) const;
	std::vector<std::string> getAvailableCommands(const GameStateView& state) const;

private:
	void registerDefaultCommands();
	static std::string normalizeName(const std::string& value);

	std::unordered_map<std::string, std::unique_ptr<Command>> commands;

protected:
};

#endif
