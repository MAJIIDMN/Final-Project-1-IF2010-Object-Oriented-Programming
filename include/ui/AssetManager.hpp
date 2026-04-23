#pragma once

#include <map>
#include <string>

#if NIMONSPOLY_ENABLE_SFML
#include <SFML/Graphics.hpp>
#endif

class AssetManager {
public:
    static AssetManager& get();

    AssetManager(const AssetManager&)            = delete;
    AssetManager& operator=(const AssetManager&) = delete;

#if NIMONSPOLY_ENABLE_SFML
    const sf::Font& font(const std::string& key);
    const sf::Texture* tileTexture(const std::string& code);
    const sf::Texture* texture(const std::string& path);
#endif

    void loadAll();

private:
    AssetManager() = default;

    bool loaded_{false};

#if NIMONSPOLY_ENABLE_SFML
    std::map<std::string, sf::Font>    fonts_;
    std::map<std::string, sf::Texture> textures_;

    void loadFonts();
    void loadTiles();

    bool tryLoad(sf::Texture& tex, const std::initializer_list<std::string>& paths);
#endif
};
