#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "core/TextFileRepository.hpp"
#include "core/engine/header/GameEngine.hpp"
#include "core/state/header/GameStateView.hpp"
#include "controllers/ComputerController.hpp"
#include "controllers/HumanController.hpp"
#include "models/Money.hpp"
#include "models/Player.hpp"
#include "ui/AppScreen.hpp"
#include "ui/AssetManager.hpp"
#include "ui/GUIInput.hpp"
#include "ui/GUIView.hpp"
#include "ui/RaylibCompat.hpp"
#include "views/gui/GuiMenuLayout.hpp"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1440, 1024, "NIMONSPOLY");
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    AssetManager::get().loadAll();

    GameEngine engine;
    TextFileRepository repo;
    engine.setRepository(&repo);
    engine.loadConfiguration("config");

    GUIView view;
    GUIInput input;
    GameStateView state(engine.getState());

    bool gameInitialized = false;
    std::vector<std::unique_ptr<Player>> ownedPlayers;
    std::vector<std::unique_ptr<PlayerController>> ownedControllers;
    std::vector<Player*> players;
    std::string pendingCommand;

    auto renderFrame = [&]() {
        BeginDrawing();
        if (view.screen() == AppScreen::IN_GAME && gameInitialized) {
            state.refresh(engine.getState(), &engine.getBoard());
            view.showTransactionLog(engine.getTransactionLogger().getFullLog());
            view.setCurrentPrompt(&input.currentPrompt());
            view.showBoard(state);
        } else {
            view.renderCurrentScreen();
        }
        EndDrawing();
    };

    input.setRenderCallback(renderFrame);

    auto startNewGame = [&]() {
        ownedPlayers.clear();
        ownedControllers.clear();
        players.clear();
        pendingCommand.clear();
        gui::menu::finalizeSetupPlayers(view.setup());

        ownedPlayers.reserve(static_cast<size_t>(view.setup().numPlayers));
        ownedControllers.reserve(static_cast<size_t>(view.setup().numPlayers));
        players.reserve(static_cast<size_t>(view.setup().numPlayers));

        for (int i = 0; i < view.setup().numPlayers; ++i) {
            std::string name =
                (i < static_cast<int>(view.setup().playerNames.size()) &&
                 !view.setup().playerNames[static_cast<size_t>(i)].empty())
                    ? view.setup().playerNames[static_cast<size_t>(i)]
                    : "P" + std::to_string(i + 1);

            if (i >= static_cast<int>(view.setup().playerNames.size())) {
                view.setup().playerNames.resize(static_cast<size_t>(i) + 1);
            }
            view.setup().playerNames[static_cast<size_t>(i)] = name;

            const bool isComputer =
                i < static_cast<int>(view.setup().isComputer.size()) &&
                view.setup().isComputer[static_cast<size_t>(i)];

            std::unique_ptr<PlayerController> controller;
            if (isComputer) {
                controller = std::make_unique<ComputerController>(name);
            } else {
                controller = std::make_unique<HumanController>(&input, name);
            }

            ownedPlayers.push_back(std::make_unique<Player>(
                name, Money(engine.getConfig().getStartingMoney()), controller.get()));
            ownedControllers.push_back(std::move(controller));
            players.push_back(ownedPlayers.back().get());
        }

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(players.begin(), players.end(), g);

        engine.initialize(players, engine.getConfig().getMaxTurns());
        engine.start();
        gameInitialized = true;
    };

    auto loadGameFromFile = [&](const std::string& path) {
        ownedPlayers.clear();
        ownedControllers.clear();
        players.clear();
        pendingCommand.clear();

        if (!repo.exists(path)) {
            return false;
        }

        const std::vector<std::string> names = repo.getPlayerNames(path);
        if (names.empty()) {
            return false;
        }

        ownedPlayers.reserve(names.size());
        ownedControllers.reserve(names.size());
        players.reserve(names.size());
        view.setup().numPlayers = static_cast<int>(names.size());
        view.setup().playerNames.clear();
        view.setup().playerNames.reserve(names.size());
        view.setup().isComputer.assign(names.size(), false);
        view.setup().playerColors.clear();
        view.setup().playerCharacters.clear();

        for (const std::string& name : names) {
            const std::string useName =
                name.empty() ? "P" + std::to_string(players.size() + 1) : name;
            auto controller = std::make_unique<HumanController>(&input, useName);
            ownedPlayers.push_back(std::make_unique<Player>(
                useName, Money(engine.getConfig().getStartingMoney()), controller.get()));
            view.setup().playerNames.push_back(useName);
            view.setup().playerColors.push_back(static_cast<int>(players.size()) % 4);
            view.setup().playerCharacters.push_back(static_cast<int>(players.size()) % 4);
            ownedControllers.push_back(std::move(controller));
            players.push_back(ownedPlayers.back().get());
        }

        engine.initialize(players, engine.getConfig().getMaxTurns());
        engine.start();
        if (!engine.loadGame(path)) {
            return false;
        }

        gameInitialized = true;
        return true;
    };

    while (!WindowShouldClose()) {
        if (view.screen() != AppScreen::IN_GAME) {
            const bool startSignal = view.handleMenuInput();
            if (startSignal && view.screen() == AppScreen::IN_GAME) {
                gameInitialized = false;
                if (!view.setup().loadFilePath.empty()) {
                    if (!loadGameFromFile(view.setup().loadFilePath)) {
                        view.setup().loadFilePath.clear();
                        view.setup() = SetupState{};
                        view.setScreen(AppScreen::LANDING);
                    }
                } else {
                    startNewGame();
                }
            }
        } else if (gameInitialized) {
            if (input.currentPrompt().type != GUIPromptType::NONE &&
                !input.currentPrompt().resolved) {
                input.updatePrompt();
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                const Vector2 mouse = GetMousePosition();
                view.handleInGameClick(mouse.x, mouse.y, pendingCommand, state);
            }
        }

        if (view.screen() == AppScreen::IN_GAME && gameInitialized && engine.isRunning()) {
            Player* active = engine.getState().getActivePlayer();

            int activeCount = 0;
            for (Player* p : players) {
                if (p && !p->isBankrupt()) {
                    ++activeCount;
                }
            }
            if (activeCount <= 1) {
                engine.stop();
                view.setScreen(AppScreen::GAME_OVER);
                state.refresh(engine.getState(), &engine.getBoard());
                view.showBoard(state);
                renderFrame();
                continue;
            }

            if (engine.getState().getCurrentTurn() > engine.getState().getMaxTurn()) {
                engine.stop();
                view.setScreen(AppScreen::GAME_OVER);
                state.refresh(engine.getState(), &engine.getBoard());
                view.showBoard(state);
                renderFrame();
                continue;
            }

            const bool promptActive =
                input.currentPrompt().type != GUIPromptType::NONE &&
                !input.currentPrompt().resolved;

            if (promptActive) {
                renderFrame();
                continue;
            }

            if (!active) {
                engine.stop();
            } else if (active->isBankrupt()) {
                engine.processCommand("SELESAI", *active);
            } else if (!pendingCommand.empty()) {
                if (pendingCommand == "DADU" && view.isDiceAnimating()) {
                    // Wait until the overlay animation finishes.
                } else if (pendingCommand == "ATUR_DADU") {
                    const auto [d1, d2] = input.getManualDice();
                    if (d1 > 0 && d2 > 0) {
                        engine.processCommand(
                            "atur dadu " + std::to_string(d1) + " " + std::to_string(d2),
                            *active);
                        view.setQueuedManualDice(d1, d2);
                        view.showSaveLoadStatus(
                            "Roll berikutnya diatur ke " + std::to_string(d1) + " + " + std::to_string(d2) + ".");
                    } else {
                        view.showSaveLoadStatus("Atur dadu dibatalkan.");
                    }
                    pendingCommand.clear();
                } else if (pendingCommand == "SIMPAN") {
                    std::string path = input.getString("Nama berkas simpanan");
                    if (path.empty()) {
                        path = "nimonspoli_save.txt";
                    }
                    if (!engine.getState().canSave()) {
                        view.showSaveLoadStatus("Permainan belum bisa disimpan.");
                    } else if (engine.saveGame(path)) {
                        view.showSaveLoadStatus("Permainan disimpan ke " + path + ".");
                    } else {
                        view.showSaveLoadStatus("Gagal menyimpan.");
                    }
                    pendingCommand.clear();
                } else if (pendingCommand == "DADU") {
                    engine.processCommand("lempar", *active);
                    view.showDiceResult(engine.getDice().getDie1(), engine.getDice().getDie2(), active->getUsername());
                    view.clearQueuedManualDice();
                    pendingCommand.clear();
                } else if (pendingCommand == "TEBUS") {
                    std::string code = input.getPropertyCodeInput("Kode properti untuk tebus");
                    if (!code.empty()) {
                        engine.processCommand("tebus " + code, *active);
                    }
                    pendingCommand.clear();
                } else if (pendingCommand == "BANGUN") {
                    std::string code = input.getPropertyCodeInput("Kode properti untuk bangun");
                    if (!code.empty()) {
                        engine.processCommand("bangun " + code, *active);
                    }
                    pendingCommand.clear();
                } else if (pendingCommand == "GADAI") {
                    std::string code = input.getPropertyCodeInput("Kode properti untuk gadai");
                    if (!code.empty()) {
                        engine.processCommand("gadai " + code, *active);
                    }
                    pendingCommand.clear();
                } else if (pendingCommand == "KARTU") {
                    const int numCards = static_cast<int>(active->getSkillCards().size());
                    if (numCards > 0) {
                        const int choice = input.getSkillCardChoice(numCards);
                        if (choice > 0) {
                            engine.processCommand("gunakan kemampuan " + std::to_string(choice), *active);
                        }
                    } else {
                        view.showSaveLoadStatus("Tidak ada kartu kemampuan.");
                    }
                    pendingCommand.clear();
                } else {
                    engine.processCommand(pendingCommand, *active);
                    pendingCommand.clear();
                }
            } else if (dynamic_cast<ComputerController*>(active->getController()) != nullptr) {
                const std::string command = active->getController()->chooseCommand(engine.getState().toView());
                if (!command.empty()) {
                    engine.processCommand(command, *active);
                }
            }
        }

        renderFrame();
    }

    AssetManager::get().unloadAll();
    CloseWindow();
    return EXIT_SUCCESS;
}
