#pragma once

#include <cstdint>


/**
 * @brief
 * PROJECT WIDE ENUMS DI SINI YA GES BIAR GA CIRCULAR INCLUDES
 */


enum class TileType : std::uint8_t {
    STREET,
    RAILROAD,
    UTILITY,
    CHANCE,
    COMMUNITY_CHEST,
    FESTIVAL,
    TAX_PPH,
    TAX_PBM,
    GO,
    JAIL,
    FREE_PARKING,
    GO_TO_JAIL,
};

enum class PropertyStatus : std::uint8_t {
    BANK,
    OWNED,
    MORTGAGED,
};

enum class PlayerStatus : std::uint8_t {
    ACTIVE,
    BANKRUPT,
    JAILED,
};

enum class Color : std::uint8_t {
    BROWN,
    LIGHT_BLUE,
    PINK,
    ORANGE,
    RED,
    YELLOW,
    GREEN,
    DARK_BLUE,
    DEFAULT,
    GRAY,
};

enum class SkillCardType : std::uint8_t {
    MOVE,
    DISCOUNT,
    SHIELD,
    TELEPORT,
    LASSO,
    DEMOLITION,
};

enum class LandingAction : std::uint8_t {
    BUY_PROMPT,
    PAY_RENT,
    PAY_TAX,
    DRAW_CHANCE,
    DRAW_COMMUNITY,
    FESTIVAL_PROMPT,
    GO_TO_JAIL,
    NOTHING,
    AUTO_ACQUIRE,
    BANKRUPT_CHECK,
};

enum class TaxChoice : std::uint8_t {
    FLAT,
    PERCENTAGE,
};

enum class AuctionAction : std::uint8_t {
    BID,
    PASS,
};

enum class AILevel : std::uint8_t {
    BASIC,
    SMART,
};

/**
 * @brief
 * one value per concrete GameEvent subclass
 * concrete values can be finalized when events are implemented
 */
enum class EventType : std::uint16_t {
    PLAYER_MOVED,
    MONEY_CHANGED,
    PROPERTY_ACQUIRED,
    RENT_PAID,
    BUILDING_BUILT,
    MORTGAGED,
    REDEEMED,
    FESTIVAL_ACTIVATED,
    AUCTION_STARTED,
    BID_PLACED,
    CARD_DRAWN,
    SKILL_CARD_USED,
    EFFECT_APPLIED,
    BANKRUPTCY,
    TURN_STARTED,
    TURN_ENDED,
    GAME_OVER,
};
