#include "core/command/header/Command.hpp"

#include "core/state/header/GameStateView.hpp"

Command::Command(std::string name, std::string help) : name(std::move(name)), help(std::move(help)) {}

Command::~Command() = default;

bool Command::canExecute(const GameStateView&) const {
	return true;
}

const std::string& Command::getName() const {
	return name;
}

const std::string& Command::getHelp() const {
	return help;
}