#include "../../headers/utils/utils.hpp"

sf::Text Utils::getText(
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

std::tuple<sf::Vector2f, sf::Vector2f, sf::Vector2f> Utils::getRectangle(const sf::Transform &transform)
{
    auto min = transform.transformPoint({0, 0});
    auto max = transform.transformPoint({1, 1});

    auto size = max - min;

    return std::make_tuple(min, max, size);
}