#ifndef __MINEFIELD_HPP__
#define __MINEFIELD_HPP__

#include "./mine.hpp"
#include <SFML/Graphics/Drawable.hpp>

class Minefield : public sf::Drawable 
{

    /**
     * A collection of mines that represents a grid
     * 
     * It will often be indexed by an x- and y- coordinate
     */
    std::vector<Mine> mines;

    /**
     * Sets some of the bombs to mines based on the 
     * number of bombs passed in. Should be used when all
     * the mines aren't bombs.
     */
    void fillMines();

public:

    // Board functions

    /**
     * Information about the size of the minefield and the 
     * number of bombs.
     */
    const std::size_t cols, rows, bombs;

    /**
     * Creates a minefield at a certain size and with a certain
     * number of bombs.
     */
    Minefield(std::size_t cols, std::size_t rows, std::size_t bombs);

    /**
     * Check if a grid x- and y- coordinate are in bounds
     */
    bool inBounds(int x, int y) const;

    /**
     * Mutable and immutable getters for a mine
     */
    const Mine& get(int x, int y) const;
    Mine& get(int x, int y);

    /**
     * Gets the number of bombs surrounding a grid position
     * given its x- and y- position
     */
    int getNeighbors(int x, int y) const;

    // Gameplay functions

    /**
     * Reveals a mine at a grid position by updating its state.
     * If the mine was a bomb, returns true.
     * 
     * If the mine had no surrounding bombs, it tries to reveal
     * more surroudning mines (that also have no neighboring bombs).
     */
    bool reveal(int x, int y);

    /**
     * Reveals all mines. Simply sets their state to discovered.
     * 
     * Usually used at the end of the game to show results. Doesn't 
     * calculate neighbors.
     */
    void revealAll();

    /**
     * Resets all mines and reassigns mines.
     */
    void resetAll();

    /**
     * Toggles a flag on a grid location. Returns true if the mine
     * was already discovered, meaning it can't be flagged.
     */ 
    bool flag(int x, int y);

public:

    // Rendering functions

    /*************
     * RENDERING *
     ************/

    /**
     * Returns a transform that represents a singular mine at (0,0). Should
     * be given the transform that represents the entire board.
     */
    sf::Transform getMineTransform(const sf::Transform& transform) const;
    
    /**
     * Returns a transform that represents a singular mine at (0,0). Should
     * be given two vectors (position and size) that represent the board.
     */
    sf::Transform getMineTransform(const sf::Vector2f& top, const sf::Vector2f& size) const;

    /**
     * Draws all the mines to the board. Is given a transform for the entire
     * board and calculates each mines 
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

#endif