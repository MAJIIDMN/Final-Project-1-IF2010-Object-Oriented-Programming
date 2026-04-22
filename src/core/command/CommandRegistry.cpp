#include "core/command/header/CommandRegistry.hpp"

#include <algorithm>
#include <cctype>

#include "core/command/header/command/BasicCommands.hpp"
#include "core/engine/header/GameEngine.hpp"
#include "core/state/header/GameStateView.hpp"
#include "models/Player.hpp"

CommandRegistry::CommandRegistry() {
	registerDefaultCommands();
}

void CommandRegistry::registerCommand(std::unique_ptr<Command> command) {
	if (!command) {
		return;
	}

	const std::string normalizedName = normalizeName(command->getName());
	if (normalizedName.empty()) {
		return;
	}

	commands[normalizedName] = std::move(command);
}

bool CommandRegistry::hasCommand(const std::string& name) const {
	return commands.find(normalizeName(name)) != commands.end();
}

bool CommandRegistry::execute(const std::string& name, GameEngine& engine, Player& player) const {
	auto it = commands.find(normalizeName(name));
	if (it == commands.end() || !it->second) {
		return false;
	}

	const GameStateView stateView = engine.getState().toView();
	if (!it->second->canExecute(stateView)) {
		return false;
	}

	it->second->execute(engine, player);
	return true;
}

bool CommandRegistry::parseAndExecute(const std::string& input, GameEngine& engine, Player& player) const {
	const std::string normalizedInput = normalizeName(input);
	if (normalizedInput.empty()) {
		return false;
	}

	const std::size_t firstSpace = normalizedInput.find(' ');
	const std::string commandName = firstSpace == std::string::npos ? normalizedInput : normalizedInput.substr(0, firstSpace);
	return execute(commandName, engine, player);
}

std::vector<std::string> CommandRegistry::getAvailableCommands(const GameStateView& state) const {
	std::vector<std::string> available;
	available.reserve(commands.size());

	for (const auto& entry : commands) {
		if (entry.second && entry.second->canExecute(state)) {
			available.push_back(entry.first);
		}
	}

	std::sort(available.begin(), available.end());
	return available;
}

void CommandRegistry::registerDefaultCommands() {
	registerCommand(std::make_unique<RollDiceCommand>());
	registerCommand(std::make_unique<EndTurnCommand>());
	registerCommand(std::make_unique<PrintBoardCommand>());
	registerCommand(std::make_unique<PrintLogCommand>());
}

std::string CommandRegistry::normalizeName(const std::string& value) {
	std::string normalized = value;

	const auto notSpace = [](unsigned char ch) { return !std::isspace(ch); };
	normalized.erase(normalized.begin(), std::find_if(normalized.begin(), normalized.end(), notSpace));
	normalized.erase(std::find_if(normalized.rbegin(), normalized.rend(), notSpace).base(), normalized.end());

	std::transform(normalized.begin(), normalized.end(), normalized.begin(),
		[](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

	return normalized;
}
