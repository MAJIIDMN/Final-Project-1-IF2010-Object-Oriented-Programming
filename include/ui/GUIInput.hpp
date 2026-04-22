#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "ui/IGameInput.hpp"

// Called every frame while GUIInput is blocking on a prompt.
// Lets the caller keep rendering without a separate thread.
using GUIRenderCallback = std::function<void()>;

namespace sf {
class RenderWindow;
class Event;
}

// ── Prompt types ──────────────────────────────────────────────────────────────
// GUIInput keeps one active prompt at a time. GameEngine methods block
// (spin-poll) until the player resolves the prompt via the GUI.

enum class GUIPromptType {
    NONE,
    COMMAND,         // free text command (> ...)
    YES_NO,          // yes / no dialog
    MENU_CHOICE,     // numbered list selection
    NUMBER,          // integer in range
    TEXT,            // free string
    DICE_MANUAL,     // two dice values
    AUCTION,         // PASS / BID <amount>
    TAX_CHOICE,      // 1 or 2
    LIQUIDATION,     // pick option index
    SKILL_CARD,      // pick card index
    PROPERTY_CODE,   // tile code string
    PLAYER_COUNT,    // 2..4 during setup
    PLAYER_NAME,     // free string during setup
};

struct GUIPromptState {
    GUIPromptType type{GUIPromptType::NONE};
    string         label;          // text shown in the overlay
    int            minVal{0};
    int            maxVal{0};
    vector<string> options;
    string         textBuffer;     // current text the player is typing

    bool           resolved{false};
    // Resolved values (only one is meaningful per type)
    bool           resBool{false};
    int            resInt{0};
    string         resString;
    AuctionDecision resAuction{};
};

class GUIInput final : public IGameInput {
public:
    explicit GUIInput(sf::RenderWindow& window);

    // Called from the render/event loop to forward SFML events to the active
    // prompt. Returns true if the prompt was resolved this frame.
    bool handleEvent(const sf::Event& event);

    // Expose the current prompt so GUIView can render the overlay.
    const GUIPromptState& currentPrompt() const;

    // Optional: called every frame inside waitForResolution() so the caller
    // can keep rendering while the game-engine thread blocks on input.
    void setRenderCallback(GUIRenderCallback cb);

    // ── IGameInput ────────────────────────────────────────────────────────────
    int    getPlayerCount() override;
    string getPlayerName(int playerIdx) override;

    string getCommand() override;
    int    getMenuChoice(const vector<string>& options) override;
    bool   getYesNo(const string& prompt) override;
    int    getNumberInRange(const string& prompt, int min, int max) override;
    string getString(const string& prompt) override;

    pair<int, int> getManualDice() override;

    AuctionDecision getAuctionDecision(
        const string& playerName, int currentBid, int playerMoney) override;

    TaxChoice getTaxChoice() override;
    int       getLiquidationChoice(int numOptions) override;
    int       getSkillCardChoice(int numCards) override;
    string    getPropertyCodeInput(const string& prompt) override;

private:
    [[maybe_unused]] sf::RenderWindow* window_;
    GUIPromptState    prompt_;
    GUIRenderCallback renderCallback_;

    // Blocks until prompt_.resolved == true while pumping SFML events.
    void waitForResolution();

    // Activate a new prompt (clears previous state).
    void activatePrompt(GUIPromptType type, const string& label,
                        int minVal = 0, int maxVal = 0,
                        const vector<string>& options = {});

    // Handle text input / confirm / backspace for the active prompt.
    void handleTextEvent(const sf::Event& event);
    void confirmPrompt();
};
