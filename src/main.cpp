#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#if NIMONSPOLY_ENABLE_SFML
#include "core/state/header/GameStateView.hpp"
#include "ui/AppScreen.hpp"
#include "ui/AssetManager.hpp"
#include "ui/GUIInput.hpp"
#include "ui/GUIView.hpp"

#include <SFML/Graphics.hpp>

#include <optional>
#else
#include "controllers/HumanController.hpp"
#include "core/engine/header/GameEngine.hpp"
#include "models/Player.hpp"
#include "ui/ConsoleInput.hpp"
#endif

int main() {
#if NIMONSPOLY_ENABLE_SFML
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{1440u, 900u}), "NIMONSPOLY");
    window.setFramerateLimit(60);

    AssetManager::get().loadAll();

    GUIView view(window);
    GUIInput input(window);
    GameStateView demoState;

    input.setRenderCallback([&]() {
        if (view.screen() == AppScreen::IN_GAME)
            view.showBoard(demoState);
        else
            view.renderCurrentScreen();
    });

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                break;
            }

            if (view.screen() != AppScreen::IN_GAME) {
                view.handleMenuEvent(*event);
            } else {
                input.handleEvent(*event);
            }
        }

        if (!window.isOpen()) break;

        if (view.screen() == AppScreen::IN_GAME)
            view.showBoard(demoState);
        else
            view.renderCurrentScreen();
    }

    return EXIT_SUCCESS;
#else
    ConsoleInput input;
    HumanController controller(&input);
    GameEngine engine;
    engine.loadConfiguration("config");

    const int playerCount = input.getNumberInRange("Jumlah pemain", 2, 4);
    std::vector<std::unique_ptr<Player>> ownedPlayers;
    std::vector<Player*> players;
    ownedPlayers.reserve(playerCount);
    players.reserve(playerCount);

    for (int i = 0; i < playerCount; ++i) {
        const std::string name = input.getString("Nama pemain " + std::to_string(i + 1));
        ownedPlayers.push_back(std::make_unique<Player>(name.empty() ? "P" + std::to_string(i + 1) : name,
                                                        Money(engine.getConfig().getStartingMoney()),
                                                        &controller));
        players.push_back(ownedPlayers.back().get());
    }

    engine.initialize(players, engine.getConfig().getMaxTurns());
    engine.start();

    while (engine.isRunning()) {
        if (engine.getState().getCurrentTurn() > engine.getState().getMaxTurn()) {
            engine.stop();
            break;
        }

        int activeCount = 0;
        Player* winner = nullptr;
        for (Player* player : players) {
            if (player && !player->isBankrupt()) {
                ++activeCount;
                winner = player;
            }
        }
        if (activeCount <= 1) {
            if (winner) {
                std::cout << "Pemenang: " << winner->getUsername() << "\n";
            }
            engine.stop();
            break;
        }

        Player* active = engine.getState().getActivePlayer();
        if (!active) {
            engine.stop();
            break;
        }
        if (active->isBankrupt()) {
            engine.processCommand("SELESAI", *active);
            continue;
        }

        std::cout << "\nTurn " << engine.getState().getCurrentTurn() << "/"
                  << engine.getState().getMaxTurn() << " - " << active->getUsername()
                  << " (" << active->getMoney().toString() << ")\n";
        const std::string command = input.getCommand();
        if (command == "quit" || command == "QUIT" || command == "keluar") {
            engine.stop();
            break;
        }
        engine.processCommand(command, *active);
    }

    return EXIT_SUCCESS;
#endif
}
