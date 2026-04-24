#pragma once

#if NIMONSPOLY_ENABLE_RAYLIB
#include <algorithm>
#include <array>
#include <string>
#include <utility>
#include <vector>

#include "ui/AppScreen.hpp"
#include "ui/RaylibCompat.hpp"

namespace gui::menu {
    constexpr int kSetupColorCount = 4;
    constexpr int kCharacterCount = 4;

    inline RaylibColor makeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
        return RaylibColor{r, g, b, a};
    }

    inline const std::array<RaylibColor, kSetupColorCount>& setupPalette() {
        static const std::array<RaylibColor, kSetupColorCount> colors{
            makeColor(0xe3, 0x4b, 0x4b),
            makeColor(0xf4, 0xd3, 0x1c),
            makeColor(0x41, 0x8d, 0xff),
            makeColor(0x32, 0xc4, 0x71),
        };
        return colors;
    }

    inline const std::array<const char*, kSetupColorCount>& setupColorKeys() {
        static const std::array<const char*, kSetupColorCount> keys{
            "red", "yellow", "blue", "green",
        };
        return keys;
    }

    inline const std::array<const char*, kCharacterCount>& characterKeys() {
        static const std::array<const char*, kCharacterCount> keys{
            "cruiser", "rocket", "shuttle", "ufo",
        };
        return keys;
    }

    inline const std::array<const char*, kCharacterCount>& characterLabels() {
        static const std::array<const char*, kCharacterCount> labels{
            "Cruiser", "Rocket", "Shuttle", "UFO",
        };
        return labels;
    }

    inline int normalizeSetupIndex(int value, int modulo) {
        if (modulo <= 0) {
            return 0;
        }
        return (value % modulo + modulo) % modulo;
    }

    inline bool hasValidSetupIndex(int value, int limit) {
        return value >= 0 && value < limit;
    }

    inline int selectedPlayerColor(const SetupState& setup, int playerIndex) {
        if (playerIndex >= 0 && playerIndex < static_cast<int>(setup.playerColors.size())) {
            const int value = setup.playerColors[static_cast<size_t>(playerIndex)];
            if (hasValidSetupIndex(value, kSetupColorCount)) {
                return value;
            }
        }
        return -1;
    }

    inline int effectivePlayerColor(const SetupState& setup, int playerIndex) {
        const int chosen = selectedPlayerColor(setup, playerIndex);
        return chosen >= 0 ? chosen : normalizeSetupIndex(playerIndex, kSetupColorCount);
    }

    inline int selectedPlayerCharacter(const SetupState& setup, int playerIndex) {
        if (playerIndex >= 0 && playerIndex < static_cast<int>(setup.playerCharacters.size())) {
            return normalizeSetupIndex(setup.playerCharacters[static_cast<size_t>(playerIndex)], kCharacterCount);
        }
        return normalizeSetupIndex(playerIndex, kCharacterCount);
    }

    inline bool colorTakenByOtherPlayer(const SetupState& setup, int currentPlayerIndex, int colorIndex) {
        for (int i = 0; i < static_cast<int>(setup.playerColors.size()); ++i) {
            if (i == currentPlayerIndex) {
                continue;
            }
            const int selected = selectedPlayerColor(setup, i);
            if (selected >= 0 && selected == colorIndex) {
                return true;
            }
        }
        return false;
    }

    inline std::string colorPreviewPath(const SetupState& setup, int playerIndex, int colorIndex) {
        const int characterIndex = selectedPlayerCharacter(setup, playerIndex);
        return "assets/components/characters/" +
               std::string(characterKeys()[static_cast<size_t>(characterIndex)]) +
               "_" + setupColorKeys()[static_cast<size_t>(normalizeSetupIndex(colorIndex, kSetupColorCount))] + ".png";
    }

    inline std::string grayPreviewPath(const SetupState& setup, int playerIndex) {
        const int characterIndex = selectedPlayerCharacter(setup, playerIndex);
        return "assets/components/characters/" +
               std::string(characterKeys()[static_cast<size_t>(characterIndex)]) +
               "_gray.png";
    }

    inline void prepareSetupPlayers(SetupState& setup) {
        const size_t count = static_cast<size_t>(std::max(0, setup.numPlayers));
        setup.playerNames.resize(count);
        setup.isComputer.resize(count, false);

        std::vector<int> colors(count, -1);
        std::array<bool, kSetupColorCount> usedColors{};
        for (size_t i = 0; i < count && i < setup.playerColors.size(); ++i) {
            const int color = setup.playerColors[i];
            if (hasValidSetupIndex(color, kSetupColorCount) &&
                !usedColors[static_cast<size_t>(color)]) {
                colors[i] = color;
                usedColors[static_cast<size_t>(color)] = true;
            }
        }
        setup.playerColors = std::move(colors);

        std::vector<int> characters(count, 0);
        for (size_t i = 0; i < count; ++i) {
            if (i < setup.playerCharacters.size()) {
                characters[i] = normalizeSetupIndex(setup.playerCharacters[i], kCharacterCount);
            } else {
                characters[i] = normalizeSetupIndex(static_cast<int>(i), kCharacterCount);
            }
        }
        setup.playerCharacters = std::move(characters);
    }

    inline void finalizeSetupPlayers(SetupState& setup) {
        prepareSetupPlayers(setup);

        std::array<bool, kSetupColorCount> usedColors{};
        for (int color : setup.playerColors) {
            if (hasValidSetupIndex(color, kSetupColorCount)) {
                usedColors[static_cast<size_t>(color)] = true;
            }
        }

        for (size_t i = 0; i < setup.playerColors.size(); ++i) {
            if (hasValidSetupIndex(setup.playerColors[i], kSetupColorCount)) {
                continue;
            }
            for (int candidate = 0; candidate < kSetupColorCount; ++candidate) {
                if (!usedColors[static_cast<size_t>(candidate)]) {
                    setup.playerColors[i] = candidate;
                    usedColors[static_cast<size_t>(candidate)] = true;
                    break;
                }
            }
            if (!hasValidSetupIndex(setup.playerColors[i], kSetupColorCount)) {
                setup.playerColors[i] = normalizeSetupIndex(static_cast<int>(i), kSetupColorCount);
            }
        }
    }

    inline Vector2 rectCenter(const Rectangle& rect) {
        return Vector2{rect.x + rect.width * 0.5f, rect.y + rect.height * 0.5f};
    }

    inline Rectangle centeredRect(float centerX, float centerY, float width, float height) {
        return Rectangle{centerX - width * 0.5f, centerY - height * 0.5f, width, height};
    }

    struct NumPlayersLayout {
        Rectangle panel{};
        Rectangle backButton{};
        std::array<Rectangle, 3> optionButtons{};
        Rectangle nextButton{};
        float titleY{0.f};
    };

    inline NumPlayersLayout makeNumPlayersLayout(float W, float H) {
        NumPlayersLayout layout{};
        const float buttonW = std::min(W * 0.28f, 370.f);
        const float buttonH = std::min(H * 0.065f, 68.f);
        const float gap = std::min(H * 0.024f, 26.f);
        const float nextButtonH = std::min(H * 0.055f, 56.f);
        const float titleH = std::min(H * 0.052f, 52.f);
        const float buttonX = W * 0.5f;
        constexpr float kPadTop = 24.f;
        constexpr float kPadBottom = 28.f;
        constexpr float kTitleGap = 28.f;
        constexpr float kNextGap = 24.f;

        const float contentH = kPadTop + titleH + kTitleGap +
                               3.f * buttonH + 2.f * gap + kNextGap +
                               nextButtonH + kPadBottom;
        const float panelW = std::min(W * 0.40f, 560.f);
        const float panelH = std::max(contentH, std::min(H * 0.52f, 500.f));
        const float panelX = W * 0.5f - panelW * 0.5f;
        const float panelY = H * 0.5f - panelH * 0.5f;
        layout.panel = {panelX, panelY, panelW, panelH};

        layout.titleY = panelY + kPadTop + titleH * 0.5f;

        const float optionTop = panelY + kPadTop + titleH + kTitleGap;
        for (int i = 0; i < 3; ++i) {
            const float cy = optionTop + buttonH * 0.5f + i * (buttonH + gap);
            layout.optionButtons[static_cast<size_t>(i)] = centeredRect(buttonX, cy, buttonW, buttonH);
        }

        const float lastOptionBottom = optionTop + 3.f * buttonH + 2.f * gap;
        layout.nextButton = centeredRect(buttonX,
                                         lastOptionBottom + kNextGap + nextButtonH * 0.5f,
                                         166.f, nextButtonH);

        layout.backButton = centeredRect(panelX + 66.f,
                                         panelY - std::min(H * 0.045f, 44.f) - 8.f,
                                         128.f, std::min(H * 0.05f, 50.f));
        return layout;
    }

    struct CustomizePlayerLayout {
        Rectangle sidePanel{};
        Rectangle mainPanel{};
        Rectangle backButton{};
        Rectangle nextButton{};
        Rectangle nameBox{};
        std::array<Rectangle, kSetupColorCount> colorSlots{};
        Rectangle characterLeft{};
        Rectangle characterRight{};
        Rectangle characterPreview{};
        Rectangle humanButton{};
        Rectangle computerButton{};
        Vector2 tabSize{};
        float tabStartY{0.f};
        float tabGap{0.f};
        float nameLabelY{0.f};
        float colorLabelY{0.f};
        float characterLabelY{0.f};
        float typeLabelY{0.f};
    };

    inline CustomizePlayerLayout makeCustomizePlayerLayout(float W, float H, int numPlayers) {
        CustomizePlayerLayout layout{};

        const float mainPanelW = std::min(W * 0.48f, 720.f);
        const float sidePanelW = std::min(W * 0.12f, 150.f);
        const float gapX = std::min(W * 0.03f, 42.f);
        const float padX = std::min(mainPanelW * 0.08f, 72.f);
        const float contentW = mainPanelW - padX * 2.f;
        const float nameBoxH = std::min(H * 0.055f, 54.f);
        const float colorGap = std::min(W * 0.012f, 18.f);
        const float colorSlotW = (contentW - colorGap * 3.f) / 4.f;
        const float colorSlotH = std::min(H * 0.11f, 108.f);
        const float charPreviewW = std::min(contentW * 0.36f, 250.f);
        const float charPreviewH = std::min(H * 0.12f, 124.f);
        const float arrowW = std::min(W * 0.045f, 66.f);
        const float arrowH = charPreviewH * 0.78f;
        const float typeGap = std::min(W * 0.02f, 22.f);
        const float typeButtonW = (contentW - typeGap) * 0.5f;
        const float typeButtonH = std::min(H * 0.055f, 54.f);
        const float nextButtonH = std::min(H * 0.05f, 52.f);

        constexpr float kLabelH = 16.f;
        constexpr float kLabelGap = 18.f;
        constexpr float kSectionGap = 22.f;
        constexpr float kPadTop = 68.f;
        constexpr float kPadBottom = 28.f;

        float rel = kPadTop;
        const float nameLabelRel = rel;  rel += kLabelH + kLabelGap;
        const float nameBoxRel   = rel;  rel += nameBoxH + kSectionGap;
        const float colorLabelRel = rel; rel += kLabelH + kLabelGap;
        const float colorTopRel  = rel;  rel += colorSlotH + kSectionGap;
        const float charLabelRel = rel;  rel += kLabelH + kLabelGap;
        const float charTopRel   = rel;  rel += charPreviewH + kSectionGap;
        const float typeLabelRel = rel;  rel += kLabelH + kLabelGap;
        const float typeTopRel   = rel;  rel += typeButtonH + kSectionGap;
        const float nextBtnRel   = rel;  rel += nextButtonH + kPadBottom;

        const float mainPanelH = std::max(rel, std::min(H * 0.68f, 720.f));
        const float totalW = sidePanelW + gapX + mainPanelW;
        const float left = W * 0.5f - totalW * 0.5f;
        const float top = std::min(H * 0.19f, std::max(20.f, H - mainPanelH - 40.f));

        layout.sidePanel = {left, top, sidePanelW, mainPanelH};
        layout.mainPanel = {left + sidePanelW + gapX, top, mainPanelW, mainPanelH};

        const float cLeft = layout.mainPanel.x + padX;
        const float panelTop = layout.mainPanel.y;

        layout.nameLabelY = panelTop + nameLabelRel;
        layout.nameBox = {cLeft, panelTop + nameBoxRel, contentW, nameBoxH};

        layout.colorLabelY = panelTop + colorLabelRel;
        const float colorAbsTop = panelTop + colorTopRel;
        for (int i = 0; i < kSetupColorCount; ++i) {
            layout.colorSlots[static_cast<size_t>(i)] = {
                cLeft + i * (colorSlotW + colorGap),
                colorAbsTop, colorSlotW, colorSlotH,
            };
        }

        layout.characterLabelY = panelTop + charLabelRel;
        const float charAbsTop = panelTop + charTopRel;
        layout.characterPreview = centeredRect(
            layout.mainPanel.x + mainPanelW * 0.5f,
            charAbsTop + charPreviewH * 0.5f,
            charPreviewW, charPreviewH);
        layout.characterLeft = centeredRect(
            layout.characterPreview.x - arrowW * 0.5f - 18.f,
            rectCenter(layout.characterPreview).y,
            arrowW, arrowH);
        layout.characterRight = centeredRect(
            layout.characterPreview.x + layout.characterPreview.width + arrowW * 0.5f + 18.f,
            rectCenter(layout.characterPreview).y,
            arrowW, arrowH);

        layout.typeLabelY = panelTop + typeLabelRel;
        const float typeAbsTop = panelTop + typeTopRel;
        layout.humanButton = {cLeft, typeAbsTop, typeButtonW, typeButtonH};
        layout.computerButton = {cLeft + typeButtonW + typeGap, typeAbsTop, typeButtonW, typeButtonH};

        layout.nextButton = centeredRect(
            layout.mainPanel.x + mainPanelW - 78.f,
            panelTop + nextBtnRel + nextButtonH * 0.5f,
            156.f, nextButtonH);

        layout.backButton = centeredRect(
            layout.sidePanel.x + layout.sidePanel.width * 0.5f,
            H * 0.10f,
            128.f, std::min(H * 0.05f, 52.f));

        const float railPad = std::min(H * 0.018f, 18.f);
        layout.tabGap = std::min(H * 0.012f, 14.f);
        const int safePlayers = std::max(1, numPlayers);
        const float railInnerH = mainPanelH - railPad * 2.f;
        const float rawTabH = (railInnerH - layout.tabGap * static_cast<float>(safePlayers - 1)) /
                               static_cast<float>(safePlayers);
        layout.tabSize = Vector2{
            sidePanelW - railPad * 2.f,
            std::clamp(rawTabH, 58.f, 86.f),
        };
        const float totalTabsH = layout.tabSize.y * static_cast<float>(safePlayers) +
                                  layout.tabGap * static_cast<float>(safePlayers - 1);
        layout.tabStartY = top + (mainPanelH - totalTabsH) * 0.5f;

        return layout;
    }

    inline Rectangle playerTabRect(const CustomizePlayerLayout& layout, int playerIndex) {
        return Rectangle{
            layout.sidePanel.x + (layout.sidePanel.width - layout.tabSize.x) * 0.5f,
            layout.tabStartY + playerIndex * (layout.tabSize.y + layout.tabGap),
            layout.tabSize.x,
            layout.tabSize.y,
        };
    }
}
#endif
