#include "../headers/minesweeper.hpp"
#include "../headers/utils/utils.hpp"
#include "../headers/utils/font_loader.hpp"

#include <SFML/Window/Event.hpp>
#include <iostream>
// Constructor

Minesweeper::Minesweeper(
    std::size_t width, std::size_t height, 
    std::size_t cols,  std::size_t rows,
    std::size_t bombs
) :
    window { 
        sf::VideoMode(width, height), 
        "Minesweeper",
        sf::Style::None 
    },
    board { cols, rows, bombs }
{
}

// MAIN FUNCTION

void Minesweeper::execute()
{
    handleInput();
    draw();
}

void Minesweeper::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        // handleEvent(event);

        // EXIT FUNCTIONS

        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // KEYBOARD FUNCTIONALITY

        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            else if (event.key.code == sf::Keyboard::Space) {
                if (state == GameState::RESET) {
                    start();
                } else if (state == GameState::LOST) {
                    reset();
                } else if (state == GameState::PLAYING) {
                    lose();
                }
            }
        }
        
        // MOUSE FUNCTIONALITY

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (state == GameState::PLAYING)
                onClick(event);
        }

    }
}

void Minesweeper::draw() 
{
    window.clear(sf::Color::White);
    drawBoard();
    drawMenu();
    window.display();
}

// BOARD DRAWING UTILS

sf::Transform Minesweeper::getBoardTransform() const
{
    auto [ width, height ] = window.getSize();
    
    sf::Transform transform;
    transform.translate(0 * 1.0f, height * top);
    transform.scale(width * 1.0f, height * (1 - top));

    return transform;
}

void Minesweeper::drawBoard()
{
    // Transform for the board
    sf::Transform transform = getBoardTransform();

    window.draw(board, transform);
}

// MENU UTILS

sf::Transform Minesweeper::getMenuTransform() const
{
    auto [ width, height ] = window.getSize();

    sf::Transform tfm;
    tfm.scale(width * 1.0f, height * top);

    return tfm;
}

inline double Minesweeper::getTimeSeconds() const
{
    if (state == GameState::LOST)
        return lastTime;
    else if (state == GameState::PLAYING)
        return timer.getElapsedTime().asSeconds();
    else 
        return 0.0;
}

void Minesweeper::drawMenu()
{

    auto fontLoad = FontLoader::load("./resources/source-code.ttf");

    auto [ top, bottom, size ] = Utils::getRectangle(getMenuTransform());

    if (fontLoad.has_value()) {
        auto text = Utils::getText(
            std::to_string(getTimeSeconds()),
            *fontLoad,
            size.y,
            sf::Color::Black,
            { 0, -0.2f * size.y }// arbitrary position fixing
        );

        window.draw(text);
    }

}

// EVENT UTILS

inline sf::Vector2i Minesweeper::getMineIndex(const sf::Vector2f in) const
{
    sf::Transform tfm = board.getMineTransform(getBoardTransform());
    tfm = tfm.getInverse();

    return static_cast<sf::Vector2i>(
        tfm.transformPoint(in)
    );
}

void Minesweeper::onClick(const sf::Event &event)
{
    auto [x, y] = getMineIndex({
        (float) event.mouseButton.x,
        (float) event.mouseButton.y
    });

    if (!board.inBounds(x, y))
        std::cout << "Mine is out of bounds " << x << ", " << y << "!\n";

    else {

        if (event.mouseButton.button == sf::Mouse::Left) {
            auto bomb = board.reveal(x, y);

            if (clicks == 0) {
                while (bomb) {
                    board.resetAll();
                    bomb = board.reveal(x, y);
                }
            } 

            else if (bomb)
                lose();
        }

        else if (event.mouseButton.button == sf::Mouse::Right) {
            bool flagged = board.flag(x, y);
        }

        clicks++;

    }

}

// GAME STATE SYSTEM

inline void Minesweeper::lose()
{
    lastTime = getTimeSeconds();
    
    state = GameState::LOST;

    // ON LOSE
    std::cout << "Clicked on a mine!\n";
    board.revealAll();
}


inline void Minesweeper::reset()
{
    state = GameState::RESET;

    clicks = 0;
    board.resetAll();
}

inline void Minesweeper::start()
{
    state = GameState::PLAYING;
    timer.restart();
}

bool Minesweeper::isPlaying() const
{
    return window.isOpen();
}