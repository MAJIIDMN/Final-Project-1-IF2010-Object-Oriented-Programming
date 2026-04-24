#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include <array>
#include <string>

#include "../components/GUIViewDraw.hpp"
#include "PageShared.hpp"
#endif

void GUIView::drawCustMap() {
#if NIMONSPOLY_ENABLE_RAYLIB
    AssetManager& am = AssetManager::get();
    ClearBackground(RL_BLACK);
    gui::page::drawMenuBackground(am);

    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const float cx = W * 0.5f;
    const float cy = H * 0.5f;

    const float backBtnH  = std::min(H * 0.05f, 50.f);
    const Rectangle backRect = gui::draw::centeredRect(cx - W * 0.36f, H * 0.08f, 130.f, backBtnH);
    gui::draw::drawMenuButton(am, "< Back", backRect, hoveredItem_ == 30, false);
    gui::draw::drawCenteredText(am, "title", "Customize Map", H * 0.042f, RL_WHITE, H * 0.10f);
    gui::draw::drawCenteredText(am, "bold", "Number of Tiles", H * 0.03f, RL_WHITE, cy - H * 0.22f);

    constexpr std::array<int, 11> tileOptions{20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
    constexpr int cols = 4;
    const float btnW = H * 0.12f;
    const float btnH = H * 0.058f;
    const float gap = H * 0.015f;
    const float gridW = cols * (btnW + gap) - gap;
    const float startX = cx - gridW * 0.5f + btnW * 0.5f;
    const float startY = cy - H * 0.12f;

    for (int i = 0; i < static_cast<int>(tileOptions.size()); ++i) {
        const int col = i % cols;
        const int row = i / cols;
        const Rectangle rect = gui::draw::centeredRect(
            startX + col * (btnW + gap),
            startY + row * (btnH + gap),
            btnW,
            btnH);
        gui::draw::drawMenuButton(am,
                                  std::to_string(tileOptions[static_cast<size_t>(i)]),
                                  rect,
                                  hoveredItem_ == i,
                                  setup_.numTiles == tileOptions[static_cast<size_t>(i)]);
    }

    gui::draw::drawMenuButton(am, "Start Game",
                              gui::draw::centeredRect(cx, cy + H * 0.32f, 220.f, 62.f),
                              hoveredItem_ == 20,
                              false);
#endif
}

bool GUIView::handleMapCustomizeInput() {
#if NIMONSPOLY_ENABLE_RAYLIB
    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const float cx = W * 0.5f;
    const float cy = H * 0.5f;
    const Vector2 mouse = GetMousePosition();

    constexpr std::array<int, 11> tileOptions{20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60};
    constexpr int cols = 4;
    const float btnW = H * 0.12f;
    const float btnH = H * 0.058f;
    const float gap = H * 0.015f;
    const float gridW = cols * (btnW + gap) - gap;
    const float startX = cx - gridW * 0.5f + btnW * 0.5f;
    const float startY = cy - H * 0.12f;
    const float backBtnH2 = std::min(H * 0.05f, 50.f);
    const Rectangle backRect = gui::draw::centeredRect(cx - W * 0.36f, H * 0.08f, 130.f, backBtnH2);
    const Rectangle startRect = gui::draw::centeredRect(cx, cy + H * 0.32f, 220.f, 62.f);

    hoveredItem_ = -1;
    if (gui::page::isHovered(backRect, mouse)) {
        hoveredItem_ = 30;
    }
    for (int i = 0; i < static_cast<int>(tileOptions.size()); ++i) {
        const int col = i % cols;
        const int row = i / cols;
        const Rectangle rect = gui::draw::centeredRect(
            startX + col * (btnW + gap),
            startY + row * (btnH + gap),
            btnW,
            btnH);
        if (gui::page::isHovered(rect, mouse)) {
            hoveredItem_ = i;
        }
    }
    if (gui::page::isHovered(startRect, mouse)) {
        hoveredItem_ = 20;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        screen_ = AppScreen::NEW_GAME_CUST_PLAYER;
        hoveredItem_ = -1;
        return false;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    if (gui::page::isHovered(backRect, mouse)) {
        screen_ = AppScreen::NEW_GAME_CUST_PLAYER;
        hoveredItem_ = -1;
        return false;
    }

    for (int i = 0; i < static_cast<int>(tileOptions.size()); ++i) {
        const int col = i % cols;
        const int row = i / cols;
        const Rectangle rect = gui::draw::centeredRect(
            startX + col * (btnW + gap),
            startY + row * (btnH + gap),
            btnW,
            btnH);
        if (gui::page::isHovered(rect, mouse)) {
            setup_.numTiles = tileOptions[static_cast<size_t>(i)];
        }
    }

    if (gui::page::isHovered(startRect, mouse)) {
        screen_ = AppScreen::IN_GAME;
        return true;
    }
#endif
    return false;
}
