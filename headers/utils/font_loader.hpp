#ifndef __FONT_LOADER_HPP__
#define __FONT_LOADER_HPP__

#include <optional>
#include <string>

#include <SFML/Graphics/Font.hpp>

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

}

#endif