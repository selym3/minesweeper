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

sf::Text FontLoader::getText(
    const std::string &txt,
    const sf::Font &font,
    unsigned int txtSize,
    sf::Color color,
    sf::Vector2f position,
    sf::Text::Style style
)
{
    sf::Text text;

    text.setCharacterSize(txtSize);

    // text.setScale((float) txtSize, (float) txtSize);
    text.setPosition(position);

    text.setFont(font);
    text.setFillColor(color);
    text.setStyle(style);

    text.setString(txt);
    return text;
}