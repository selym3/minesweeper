#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <SFML/Graphics/Text.hpp> 

namespace Utils
{
    /**
     * Returns a graphical text structure that can be drawn. 
     * 
     * It takes text, font, text size, color, position, and style. 
     */
    sf::Text getText(
        const std::string &txt,
        const sf::Font &font,
        unsigned int txtSize = 12,
        sf::Color color = sf::Color::Black,
        sf::Vector2f position = sf::Vector2f{0, 0},
        sf::Text::Style style = sf::Text::Style::Regular);

    /**
    * Returns a top left, bottom right, and size of a rectangle 
    * from a geometric transform.
    * 
    * It is suggested that structured bindings are used to deconstruct
    * the tuple. 
    */
    std::tuple<sf::Vector2f, sf::Vector2f, sf::Vector2f> getRectangle(const sf::Transform &transform);

} // namespace Utils

#endif