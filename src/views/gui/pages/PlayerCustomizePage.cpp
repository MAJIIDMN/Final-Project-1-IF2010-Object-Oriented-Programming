#include "ui/GUIView.hpp"

#if NIMONSPOLY_ENABLE_RAYLIB
#include <algorithm>
#include <string>

#include "../components/GUIViewDraw.hpp"
#include "../GuiMenuLayout.hpp"
#include "PageShared.hpp"
#endif

void GUIView::drawCustPlayer() {
#if NIMONSPOLY_ENABLE_RAYLIB
    AssetManager& am = AssetManager::get();
    ClearBackground(RL_BLACK);
    gui::page::drawMenuBackground(am);

    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const gui::menu::CustomizePlayerLayout layout = gui::menu::makeCustomizePlayerLayout(W, H, setup_.numPlayers);
    const int playerIndex = std::clamp(custPlayerTab_, 0, std::max(0, setup_.numPlayers - 1));

    gui::draw::drawCenteredText(am, "title", "Customize Player", H * 0.042f, RL_WHITE, H * 0.10f);
    gui::draw::drawMenuButton(am, "< Back", layout.backButton, hoveredItem_ == 20, false);

    for (int i = 0; i < setup_.numPlayers; ++i) {
        const Rectangle tabRect = gui::menu::playerTabRect(layout, i);
        const bool selected = (i == playerIndex);
        const bool hovered = (hoveredItem_ == 100 + i);
        const int selectedTabColor = gui::menu::selectedPlayerColor(setup_, i);
        const RaylibColor accent = selectedTabColor >= 0
            ? gui::menu::setupPalette()[static_cast<size_t>(selectedTabColor)]
            : gui::menu::makeColor(0x86, 0x93, 0xa7);
        const RaylibColor fill = selected ? gui::menu::makeColor(accent.r, accent.g, accent.b, 58)
                                          : hovered ? gui::menu::makeColor(255, 255, 255, 28)
                                                    : gui::menu::makeColor(255, 255, 255, 12);
        const RaylibColor outline = selected ? accent
                                             : gui::menu::makeColor(255, 255, 255, hovered ? 110 : 50);
        gui::draw::drawPanel(tabRect, fill, outline);

        const std::string label = "P" + std::to_string(i + 1);
        const float fontSize = layout.tabSize.y * 0.44f;
        const Font& font = am.font("title");
        const Vector2 textSize = MeasureTextEx(font, label.c_str(), fontSize, 0.f);
        const Vector2 center = gui::menu::rectCenter(tabRect);
        DrawTextEx(font,
                   label.c_str(),
                   Vector2{center.x - textSize.x * 0.5f, center.y - textSize.y * 0.5f},
                   fontSize,
                   0.f,
                   selected ? RL_WHITE : gui::menu::makeColor(accent.r, accent.g, accent.b, hovered ? 230 : 185));
    }

    gui::draw::drawLabel(am, "bold", "Name", H * 0.024f, gui::menu::makeColor(236, 241, 250),
                         Vector2{layout.nameBox.x, layout.nameLabelY});
    gui::draw::drawPanel(layout.nameBox, gui::menu::makeColor(16, 22, 34, 220),
                         gui::menu::makeColor(255, 255, 255, 70));

    const std::string name =
        playerIndex < static_cast<int>(setup_.playerNames.size())
            ? setup_.playerNames[static_cast<size_t>(playerIndex)]
            : "";
    const std::string displayName = name.empty() ? "Player " + std::to_string(playerIndex + 1) : name;
    const float nameFontSize = H * 0.028f;
    const Font& titleFont = am.font("title");
    const Vector2 nameSize = MeasureTextEx(titleFont, displayName.c_str(), nameFontSize, 0.f);
    DrawTextEx(titleFont,
               displayName.c_str(),
               Vector2{
                   layout.nameBox.x + layout.nameBox.width * 0.04f,
                   layout.nameBox.y + layout.nameBox.height * 0.5f - nameSize.y * 0.5f,
               },
               nameFontSize,
               0.f,
               name.empty() ? gui::menu::makeColor(156, 166, 184) : gui::menu::makeColor(240, 245, 255));

    gui::draw::drawLabel(am, "bold", "Color", H * 0.024f, gui::menu::makeColor(236, 241, 250),
                         Vector2{layout.colorSlots[0].x, layout.colorLabelY});
    const int selectedColor = gui::menu::selectedPlayerColor(setup_, playerIndex);
    for (int ci = 0; ci < gui::menu::kSetupColorCount; ++ci) {
        const Rectangle slotRect = layout.colorSlots[static_cast<size_t>(ci)];
        const bool taken = gui::menu::colorTakenByOtherPlayer(setup_, playerIndex, ci);
        const bool selected = (ci == selectedColor);
        const bool hovered = (hoveredItem_ == 200 + ci);
        const RaylibColor outline = selected
                                        ? RL_WHITE
                                        : taken ? gui::menu::makeColor(120, 130, 145, 70)
                                                : gui::menu::makeColor(
                                                      gui::menu::setupPalette()[static_cast<size_t>(ci)].r,
                                                      gui::menu::setupPalette()[static_cast<size_t>(ci)].g,
                                                      gui::menu::setupPalette()[static_cast<size_t>(ci)].b,
                                                      hovered ? 170 : 110);
        gui::draw::drawPanel(slotRect,
                             taken && !selected ? gui::menu::makeColor(28, 33, 44, 190)
                                                : hovered ? gui::menu::makeColor(24, 31, 45, 236)
                                                          : gui::menu::makeColor(18, 24, 38, 220),
                             outline);

        const Vector2 slotCenter = gui::menu::rectCenter(slotRect);
        if (const Texture2D* tex = am.texture(gui::menu::colorPreviewPath(setup_, playerIndex, ci))) {
            const float maxW = slotRect.width * 0.72f;
            const float maxH = slotRect.height * 0.52f;
            const float scale = std::min(maxW / static_cast<float>(tex->width),
                                         maxH / static_cast<float>(tex->height));
            const float drawW = static_cast<float>(tex->width) * scale;
            const float drawH = static_cast<float>(tex->height) * scale;
            gui::draw::drawSprite(tex,
                                  Rectangle{
                                      slotCenter.x - drawW * 0.5f,
                                      slotRect.y + slotRect.height * 0.42f - drawH * 0.5f,
                                      drawW,
                                      drawH,
                                  },
                                  (taken && !selected) ? gui::menu::makeColor(115, 115, 115, 120) : RL_WHITE);
        }

        const std::string label = gui::menu::setupColorKeys()[static_cast<size_t>(ci)];
        const float labelSize = slotRect.height * 0.16f;
        const Font& regularFont = am.font("regular");
        const Vector2 textSize = MeasureTextEx(regularFont, label.c_str(), labelSize, 0.f);
        DrawTextEx(regularFont,
                   label.c_str(),
                   Vector2{
                       slotCenter.x - textSize.x * 0.5f,
                       slotRect.y + slotRect.height * 0.72f,
                   },
                   labelSize,
                   0.f,
                   taken && !selected ? gui::menu::makeColor(120, 128, 142)
                                      : gui::menu::makeColor(222, 228, 238));
    }

    if (selectedColor < 0) {
        const std::string hint = "belum dipilih";
        const float hintSize = H * 0.017f;
        const Vector2 hintMeasure = MeasureTextEx(am.font("regular"), hint.c_str(), hintSize, 0.f);
        DrawTextEx(am.font("regular"),
                   hint.c_str(),
                   Vector2{layout.mainPanel.x + layout.mainPanel.width - hintMeasure.x - 28.f, layout.colorLabelY + 2.f},
                   hintSize,
                   0.f,
                   gui::menu::makeColor(150, 162, 180));
    }

    gui::draw::drawLabel(am, "bold", "Character", H * 0.024f, gui::menu::makeColor(236, 241, 250),
                         Vector2{layout.nameBox.x, layout.characterLabelY});
    gui::draw::drawMenuButton(am, "<", layout.characterLeft, hoveredItem_ == 300, false);
    gui::draw::drawMenuButton(am, ">", layout.characterRight, hoveredItem_ == 301, false);
    gui::draw::drawPanel(layout.characterPreview, gui::menu::makeColor(18, 24, 38, 220),
                         gui::menu::makeColor(255, 255, 255, 70));
    if (const Texture2D* tex = am.texture(gui::menu::grayPreviewPath(setup_, playerIndex))) {
        const float maxW = layout.characterPreview.width * 0.62f;
        const float maxH = layout.characterPreview.height * 0.54f;
        const float scale = std::min(maxW / static_cast<float>(tex->width),
                                     maxH / static_cast<float>(tex->height));
        const float drawW = static_cast<float>(tex->width) * scale;
        const float drawH = static_cast<float>(tex->height) * scale;
        gui::draw::drawSprite(tex,
                              Rectangle{
                                  layout.characterPreview.x + layout.characterPreview.width * 0.5f - drawW * 0.5f,
                                  layout.characterPreview.y + layout.characterPreview.height * 0.40f - drawH * 0.5f,
                                  drawW,
                                  drawH,
                              });
    }

    const int selectedCharacter = gui::menu::selectedPlayerCharacter(setup_, playerIndex);
    const std::string characterLabel =
        gui::menu::characterLabels()[static_cast<size_t>(selectedCharacter)];
    const float characterFontSize = layout.characterPreview.height * 0.16f;
    const Font& regularFont = am.font("regular");
    const Vector2 characterTextSize = MeasureTextEx(regularFont, characterLabel.c_str(), characterFontSize, 0.f);
    DrawTextEx(regularFont,
               characterLabel.c_str(),
               Vector2{
                   layout.characterPreview.x + layout.characterPreview.width * 0.5f - characterTextSize.x * 0.5f,
                   layout.characterPreview.y + layout.characterPreview.height * 0.74f,
               },
               characterFontSize,
               0.f,
               gui::menu::makeColor(214, 220, 230));

    gui::draw::drawLabel(am, "bold", "Type", H * 0.024f, gui::menu::makeColor(236, 241, 250),
                         Vector2{layout.nameBox.x, layout.typeLabelY});
    const bool isComputer =
        playerIndex < static_cast<int>(setup_.isComputer.size()) &&
        setup_.isComputer[static_cast<size_t>(playerIndex)];
    gui::draw::drawMenuButton(am, "Human", layout.humanButton, hoveredItem_ == 400, !isComputer);
    gui::draw::drawMenuButton(am, "Computer", layout.computerButton, hoveredItem_ == 401, isComputer);

    gui::draw::drawMenuButton(am, "Next >", layout.nextButton, hoveredItem_ == 10, false);
#endif
}

bool GUIView::handlePlayerCustomizeInput() {
#if NIMONSPOLY_ENABLE_RAYLIB
    const float W = static_cast<float>(GetScreenWidth());
    const float H = static_cast<float>(GetScreenHeight());
    const gui::menu::CustomizePlayerLayout layout = gui::menu::makeCustomizePlayerLayout(W, H, setup_.numPlayers);
    const Vector2 mouse = GetMousePosition();

    custPlayerTab_ = std::clamp(custPlayerTab_, 0, std::max(0, setup_.numPlayers - 1));
    while (static_cast<int>(setup_.playerNames.size()) <= custPlayerTab_) {
        setup_.playerNames.emplace_back();
    }
    gui::page::appendTextInput(setup_.playerNames[static_cast<size_t>(custPlayerTab_)], 16);

    hoveredItem_ = -1;
    if (gui::page::isHovered(layout.backButton, mouse)) {
        hoveredItem_ = 20;
    }
    for (int i = 0; i < setup_.numPlayers; ++i) {
        if (gui::page::isHovered(gui::menu::playerTabRect(layout, i), mouse)) {
            hoveredItem_ = 100 + i;
        }
    }
    for (int ci = 0; ci < gui::menu::kSetupColorCount; ++ci) {
        if (gui::page::isHovered(layout.colorSlots[static_cast<size_t>(ci)], mouse)) {
            hoveredItem_ = 200 + ci;
        }
    }
    if (gui::page::isHovered(layout.characterLeft, mouse)) {
        hoveredItem_ = 300;
    } else if (gui::page::isHovered(layout.characterRight, mouse)) {
        hoveredItem_ = 301;
    } else if (gui::page::isHovered(layout.humanButton, mouse)) {
        hoveredItem_ = 400;
    } else if (gui::page::isHovered(layout.computerButton, mouse)) {
        hoveredItem_ = 401;
    } else if (gui::page::isHovered(layout.nextButton, mouse)) {
        hoveredItem_ = 10;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        screen_ = AppScreen::NEW_GAME_NUM_PLAYERS;
        hoveredItem_ = -1;
        return false;
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return false;
    }

    if (gui::page::isHovered(layout.backButton, mouse)) {
        screen_ = AppScreen::NEW_GAME_NUM_PLAYERS;
        hoveredItem_ = -1;
        return false;
    }

    for (int i = 0; i < setup_.numPlayers; ++i) {
        if (gui::page::isHovered(gui::menu::playerTabRect(layout, i), mouse)) {
            custPlayerTab_ = i;
        }
    }

    const int playerIndex = custPlayerTab_;
    for (int ci = 0; ci < gui::menu::kSetupColorCount; ++ci) {
        if (gui::page::isHovered(layout.colorSlots[static_cast<size_t>(ci)], mouse) &&
            !gui::menu::colorTakenByOtherPlayer(setup_, playerIndex, ci) &&
            playerIndex < static_cast<int>(setup_.playerColors.size())) {
            setup_.playerColors[static_cast<size_t>(playerIndex)] = ci;
        }
    }

    if (playerIndex < static_cast<int>(setup_.playerCharacters.size()) &&
        gui::page::isHovered(layout.characterLeft, mouse)) {
        const int current = gui::menu::selectedPlayerCharacter(setup_, playerIndex);
        setup_.playerCharacters[static_cast<size_t>(playerIndex)] =
            gui::menu::normalizeSetupIndex(current - 1, gui::menu::kCharacterCount);
    } else if (playerIndex < static_cast<int>(setup_.playerCharacters.size()) &&
               gui::page::isHovered(layout.characterRight, mouse)) {
        const int current = gui::menu::selectedPlayerCharacter(setup_, playerIndex);
        setup_.playerCharacters[static_cast<size_t>(playerIndex)] =
            gui::menu::normalizeSetupIndex(current + 1, gui::menu::kCharacterCount);
    }

    if (gui::page::isHovered(layout.humanButton, mouse)) {
        if (playerIndex < static_cast<int>(setup_.isComputer.size())) {
            setup_.isComputer[static_cast<size_t>(playerIndex)] = false;
        }
    } else if (gui::page::isHovered(layout.computerButton, mouse)) {
        if (playerIndex < static_cast<int>(setup_.isComputer.size())) {
            setup_.isComputer[static_cast<size_t>(playerIndex)] = true;
        }
    }

    if (gui::page::isHovered(layout.nextButton, mouse)) {
        screen_ = AppScreen::NEW_GAME_CUST_MAP;
        hoveredItem_ = -1;
    }
#endif
    return false;
}
