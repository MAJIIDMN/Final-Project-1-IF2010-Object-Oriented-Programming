#include <cstdlib>

#if NIMONSPOLY_ENABLE_SFML
#include "core/state/header/GameStateView.hpp"
#include "ui/AppScreen.hpp"
#include "ui/AssetManager.hpp"
#include "ui/GUIInput.hpp"
#include "ui/GUIView.hpp"

#include <SFML/Graphics.hpp>

#include <optional>
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
    return EXIT_SUCCESS;
#endif
}
