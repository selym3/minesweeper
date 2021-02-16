#ifndef __MINESWEEPER_HPP__
#define __MINESWEEPER_HPP__

#include "minefield.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Minesweeper 
{

    sf::RenderWindow window;

    Minefield board;

    // TODO: add more helper functions to control these properly
    bool started = false, 
            lost = false;
    sf::Clock timer;
    double lastTime;

    // Window utils

    void centerWindow();

    // Board utils

    float top = 0.05;

    sf::Transform getBoardTransform() const;

    void drawBoard();
    
    // Menu utils

    sf::Transform getMenuTransform() const;

    inline double getTimeSeconds() const;
    void drawMenu();

    // Event utils

    inline sf::Vector2i getMineIndex(const sf::Vector2f in) const;
    inline sf::Vector2i getMineIndex(const sf::Event::MouseButtonEvent& event) const;

    void onInput(const sf::Event &event);

    // Game states

    inline bool shouldRun() const;

    inline void lose();
    inline void reset();
    inline void start();

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