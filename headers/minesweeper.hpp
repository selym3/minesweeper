#ifndef __MINESWEEPER_HPP__
#define __MINESWEEPER_HPP__

#include "minefield.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

class Minesweeper 
{
private:
    Minefield board;

    /**
     * GameState data
     */

    enum GameState {
        LOST,
        PLAYING,
        RESET
    };

    GameState state = GameState::RESET;

    sf::Clock timer;
    double lastTime;

    inline double getTimeSeconds() const;

    unsigned int clicks = 0;

    inline void lose();
    inline void reset();
    inline void start();

private:

    /**
     * Event data
     */

    inline sf::Vector2i getMineIndex(const sf::Vector2f in) const;

    void onClick(const sf::Event &event);

private:
    sf::RenderWindow window;

    /**
     * Rendering data
     */

    float top = 0.05;

    // Board
    sf::Transform getBoardTransform() const;
    void drawBoard();
    
    // Menu
    sf::Transform getMenuTransform() const;
    void drawMenu();

private:

    void handleInput();
    void draw();

public:

    Minesweeper(
        std::size_t width, std::size_t height, 
        std::size_t cols,  std::size_t rows,
        std::size_t bombs
    );

    // If the program shouldn't close, isPlaying is true
    bool isPlaying() const;
    void execute();

};

#endif