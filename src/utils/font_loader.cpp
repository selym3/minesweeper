#include "../../headers/utils/font_loader.hpp"

#include <unordered_map>
#include <optional>

FontLoader::FontLoad FontLoader::load(const std::string &path)
{
    // LOOKUP TABLE
    static std::unordered_map<std::string, sf::Font> LOOKUP_TABLE;

    // ALGORITHM

    auto fontEntry = LOOKUP_TABLE.find(path);
    bool contains = fontEntry != LOOKUP_TABLE.end();

    if (contains)
    {
        return std::ref(fontEntry->second);
    }
    else
    {

        sf::Font font;

        bool loaded = font.loadFromFile(path);

        if (loaded) {
            auto entry = LOOKUP_TABLE.insert({path, font});

            return std::ref(entry.first->second);
        } else {
            return std::nullopt;
        }
    }
}