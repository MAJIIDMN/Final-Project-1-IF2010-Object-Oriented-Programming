#pragma once

#include <map>
#include <string>

#if NIMONSPOLY_ENABLE_SFML
#include <SFML/Graphics.hpp>
#endif

// Singleton texture + font cache.
// All paths are relative to the working directory (project root).
// Missing assets return nullptr / a default font — never crash.
class AssetManager {
public:
    static AssetManager& get();

    // Delete copy/move
    AssetManager(const AssetManager&)            = delete;
    AssetManager& operator=(const AssetManager&) = delete;

#if NIMONSPOLY_ENABLE_SFML
    // Fonts — keys: "bold", "extrabold", "title"
    // Returns a valid font (falls back to the first loaded font if key missing).
    const sf::Font& font(const std::string& key);

    // Tile textures — key = tile code (e.g. "JKT", "GBR", "PLN", "FES").
    // Searches property/ railroad/ utility/ special/ and root tiles/ in order.
    // Returns nullptr if no file found for this code.
    const sf::Texture* tileTexture(const std::string& code);

    // Generic texture by path relative to project root.
    // Returns nullptr if file not found.
    const sf::Texture* texture(const std::string& path);
#endif

    // Call once at startup. Safe to call multiple times (no-op if already loaded).
    void loadAll();

private:
    AssetManager() = default;

    bool loaded_{false};

#if NIMONSPOLY_ENABLE_SFML
    std::map<std::string, sf::Font>    fonts_;
    std::map<std::string, sf::Texture> textures_;

    void loadFonts();
    void loadTiles();

    // Tries each candidate path in order; returns true on first success.
    bool tryLoad(sf::Texture& tex, const std::initializer_list<std::string>& paths);
#endif
};
