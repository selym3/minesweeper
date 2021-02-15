#ifndef __FONT_LOADER_HPP__
#define __FONT_LOADER_HPP__

#include <optional>
#include <string>

#include <SFML/Graphics/Text.hpp>

namespace FontLoader
{

    /**
     * A immutable reference to a font that can be null. 
     */
    using FontLoad = std::optional<std::reference_wrapper<const sf::Font>>;

    /**
     * Loads fonts based on their bath and builds a lookup table of 
     * fonts based on their path.
     * 
     * Returns a font on success, and a null option on failure.
     */
    FontLoad load(const std::string& path);

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
        sf::Text::Style style = sf::Text::Style::Regular
    );
    
}

#endif