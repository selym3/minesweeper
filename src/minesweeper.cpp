#include "../headers/minesweeper.hpp"
#include "../headers/utils/utils.hpp"
#include "../headers/utils/font_loader.hpp"

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
    // centerWindow();
}

// MAIN FUNCTION

void Minesweeper::execute()
{
    // HANDLE USER INPUTS HERE

    sf::Event event;
    while (window.pollEvent(event)) {

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
                if (!this->started) {
                    start();
                } else if (this->lost) {
                    reset();
                } else {
                    lose();
                }
            }
        }
        
        // MOUSE FUNCTIONALITY

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (shouldRun())
                onInput(event);
        }

    }

    // RENDER HERE

    window.clear(sf::Color::White);

    drawBoard();
    drawMenu();

    window.display();
}

// WINDOW UTILS

void Minesweeper::centerWindow()
{
    auto desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(
        desktop.width / 2 - window.getSize().x / 2,
        desktop.height / 2 - window.getSize().y / 2
    ));
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
    if (lost)
        return lastTime;
    else if (started)
        return timer.getElapsedTime().asSeconds();
    // else if (lost) order matters, because started and lost can both be true
        // return lastTime;
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

inline sf::Vector2i Minesweeper::getMineIndex(const sf::Event::MouseButtonEvent& event) const
{
    return getMineIndex({
        (float) event.x,
        (float) event.y
    });
}

void Minesweeper::onInput(const sf::Event &event)
{
    auto [x, y] = getMineIndex(event.mouseButton);

    if (!board.inBounds(x, y))
        std::cout << "Mine is out of bounds " << x << ", " << y << "!\n";

    else {

        if (event.mouseButton.button == sf::Mouse::Left) {
            auto bomb = board.reveal(x, y);

            if (bomb)
                lose();
        }

        else if (event.mouseButton.button == sf::Mouse::Right) {
            bool flagged = board.flag(x, y);
        }

    }
}

// GAME STATE SYSTEM

inline bool Minesweeper::shouldRun() const
{
    return started && !lost;
}

inline void Minesweeper::lose()
{
    lastTime = getTimeSeconds();
    this->lost = true;

    // ON LOSE
    std::cout << "Clicked on a mine!\n";
    board.revealAll();
}


inline void Minesweeper::reset()
{
    started = false;
    lost = false;
    board.resetAll();
}

inline void Minesweeper::start()
{
    this->started = true;
    timer.restart();
}

bool Minesweeper::isPlaying() const
{
    return window.isOpen();
}