#include "ui/AssetManager.hpp"

#if NIMONSPOLY_ENABLE_SFML
#include <SFML/Graphics.hpp>
#include <initializer_list>
#include <iostream>
#include <string>
#endif

AssetManager& AssetManager::get() {
    static AssetManager instance;
    return instance;
}

void AssetManager::loadAll() {
    if (loaded_) return;
    loaded_ = true;
#if NIMONSPOLY_ENABLE_SFML
    loadFonts();
    loadTiles();
#endif
}

#if NIMONSPOLY_ENABLE_SFML


void AssetManager::loadFonts() {
    struct FontEntry { std::string key; std::string path; };
    const FontEntry entries[] = {
        { "bold",      "assets/fonts/GemunuLibre/GemunuLibre-Bold.ttf"      },
        { "extrabold", "assets/fonts/GemunuLibre/GemunuLibre-ExtraBold.ttf" },
        { "semibold",  "assets/fonts/GemunuLibre/GemunuLibre-SemiBold.ttf"  },
        { "regular",   "assets/fonts/GemunuLibre/GemunuLibre-Regular.ttf"   },
        { "title",     "assets/fonts/Hahmlet/hahmlet-bold.ttf"              },
    };
    for (const auto& e : entries) {
        sf::Font f;
        if (f.openFromFile(e.path)) {
            fonts_.emplace(e.key, std::move(f));
        } else {
            std::cerr << "[AssetManager] Font not found: " << e.path << "\n";
        }
    }
}

const sf::Font& AssetManager::font(const std::string& key) {
    auto it = fonts_.find(key);
    if (it != fonts_.end()) return it->second;
    // fallback: first available font
    if (!fonts_.empty()) return fonts_.begin()->second;
    // absolute last resort: static default (will look bad but won't crash)
    static sf::Font defaultFont;
    return defaultFont;
}


bool AssetManager::tryLoad(sf::Texture& tex,
                            const std::initializer_list<std::string>& paths) {
    for (const auto& p : paths) {
        if (tex.loadFromFile(p)) return true;
    }
    return false;
}

void AssetManager::loadTiles() {
    const std::string codes[] = {
        "BDG","BKS","BLP","BGR","DEN","DPK","GRT","IKN","JKT",
        "MAL","MED","MGL","MKS","MND","MTR","PKB","PLB","SBY",
        "SMG","SOL","TSK","YOG",
        "GBR","GUB","STB","TGU","TUG",
        "PAM","PLN",
        "GO","PPJ","PEN","BBP","DNU","FES","KSP","PBM","PPH",
    };

    for (const auto& code : codes) {
        sf::Texture tex;
        bool ok = tryLoad(tex, {
            "assets/tiles/property/" + code + ".png",
            "assets/tiles/railroad/"  + code + ".png",
            "assets/tiles/utility/"   + code + ".png",
            "assets/tiles/special/"   + code + ".png",
            "assets/tiles/"           + code + ".png",
        });
        if (ok) {
            tex.setSmooth(true);
            textures_.emplace(code, std::move(tex));
        }
        // silently skip missing codes — fallback handled in tileTexture()
    }
}

const sf::Texture* AssetManager::tileTexture(const std::string& code) {
    auto it = textures_.find(code);
    if (it != textures_.end()) return &it->second;
    return nullptr;
}

const sf::Texture* AssetManager::texture(const std::string& path) {
    auto it = textures_.find(path);
    if (it != textures_.end()) return &it->second;

    sf::Texture tex;
    if (tex.loadFromFile(path)) {
        tex.setSmooth(true);
        auto [ins, _] = textures_.emplace(path, std::move(tex));
        return &ins->second;
    }
    return nullptr;
}

#endif
