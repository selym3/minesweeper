#ifndef __MINE_HPP__
#define __MINE_HPP__

#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include <SFML/Graphics/Color.hpp>

struct Mine : public sf::Drawable
{
    // Game information

    /**
     * Represents the state of a mine. 
     * 
     * All of these are mutually exclusive, so they 
     * are in an enum for convenience.
     */
    enum State {
        Default,
        Discovered,
        Flagged
    };
    State state = State::Default;

    /**
     * These are convenience methods to interpret 
     * which state the mine is.
     */
    bool discovered() const;
    bool flagged() const;

    /**
     * A mine can have any state and be a bomb.
     */
    bool bomb;

    unsigned int neighbors;

    /**
     * Resets the state of a mine 
     * entirely
     */
    void reset();

public:
    // Render information
    
    /**
     * Draws the mine as a rectangle with text somewhere on the board. 
     * 
     * The transform in the render states is provided by a `Minefield`
     * and will be scaled and then translated by the `Minefield` to fit.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    /**
     * Helper method to decide what text to put and what color it should
     * be when drawing the mine.
     */
    inline std::pair<std::string, sf::Color> getInfo() const;

};

#endif