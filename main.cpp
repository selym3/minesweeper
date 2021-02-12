#include <iostream>
#include <SFML/Graphics.hpp>

template <typename T>
std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& rhs)
{
    return os << "[ " << rhs.x << ", " << rhs.y << " ]";
}

/**
 * Returns a top left, bottom right, and size of a rectangle
 */
auto getRectangle(const sf::Transform& transform)
{
    auto min = transform.transformPoint({0,0});    
    auto max = transform.transformPoint({1,1});

    auto size = max - min;

    return std::make_tuple( min, max, size );
}

struct Mine : public sf::Drawable
{

    // Game information

    bool flagged, 
        discovered, 
        bomb;

    unsigned int neighbors;

    // Render information
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        auto [ top, bottom, size ] = getRectangle(states.transform);

        // Create a drawable field
        sf::RectangleShape field {};
        field.setPosition(top);
        field.setSize(size);
        field.setFillColor(sf::Color::Blue);
        field.setOutlineColor(sf::Color::Black);
        field.setOutlineThickness(1);

        target.draw(field);
    };

};

//

#include <optional>
#include <vector>

class Minefield : public sf::Drawable 
{

    std::vector<Mine> mines;

    // Reference
    template <typename T>
    using reference = std::reference_wrapper<T>;

    // Reference with null state
    template <typename T>
    using optional_reference = std::optional<reference<T>>;

    // Const, mutable optional reference types 
    using cMine = optional_reference<const Mine>;
    using mMine = optional_reference<Mine>;

    std::size_t getIndex(int x, int y) const
    {
        return y * cols + x;
    }

    reference<const Mine> getReference(int x, int y) const
    {
        return std::ref(mines[getIndex(x, y)]);
    }

    reference<Mine> getReference(int x, int y)
    {
        return std::ref(mines[getIndex(x, y)]);
    }

public:
    const std::size_t cols, rows;

    Minefield(std::size_t cols, std::size_t rows)
        : cols { cols }, rows { rows } 
    {
        auto size = cols * rows;

        mines.resize(size);
    }

    bool inBounds(int x, int y) const
    {
        return x >= 0 && x < cols && y >= 0 && y < rows;
    }

    cMine get(int x, int y) const
    {
        return inBounds(x, y) ? cMine{ getReference(x, y)} : std::nullopt;
    }

    mMine get(int x, int y) 
    {
        return inBounds(x, y) ? mMine{ getReference(x, y) } : std::nullopt;
    }

    // Rendering function

    sf::Transform getMineTransform(const sf::Transform& transform) const
    {
        auto [ top, bottom, size ] = getRectangle(transform);

        return getMineTransform(top, size);
    }

    sf::Transform getMineTransform(const sf::Vector2f& top, const sf::Vector2f& size) const
    {
        sf::Transform mineTransform {};
        mineTransform.translate(top);
        mineTransform.scale({ size.x / (float) cols, size.y / (float) rows });

        return mineTransform;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Draw all the mines
        sf::Transform mineTransform = getMineTransform(states.transform);

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                auto mine = get(x, y);

                target.draw(*mine, sf::Transform(mineTransform).translate(x, y));
                // mineTransform.translate(1, 1);

            }   
            // mineTransform.translate(-cols, 1);

        }
    }

};

////

class Minesweeper 
{

    sf::RenderWindow window;
    std::size_t width, height; // <-- for convenience

    Minefield board;

    // Window utils

    void centerWindow()
    {
        auto desktop = sf::VideoMode::getDesktopMode();
        window.setPosition(sf::Vector2i(
            desktop.width / 2 - window.getSize().x / 2,
            desktop.height / 2 - window.getSize().y / 2
        ));
    }

    // Board utils

    float top = 0.25;

    sf::Transform getBoardTransform(float top = 0.25) const
    {
        sf::Transform transform {};
        transform.translate(0 * 1.0f, height * top);
        transform.scale(width * 1.0f, height * (1 - top));

        return transform;
    }

    void drawBoard()
    {
        // Transform for the board
        sf::Transform transform = getBoardTransform(this->top);

        window.draw(board, transform);
    }

    // Menu utils

    void drawMenu()
    {

    }

public:

    Minesweeper(
        std::size_t width, std::size_t height, 
        std::size_t cols,  std::size_t rows
    ) :
        window { 
            sf::VideoMode(width, height), 
            "Minesweeper", 
            sf::Style::None 
        },
        width { width }, height { height },
        board { cols , rows }
    {
        centerWindow();
    }

    bool isRunning() const
    {
        return window.isOpen();
    }

    void execute()
    {
        // HANDLE KEY INPUTS HERE

        auto getMouseFromEvent = [](auto eventType) {
            return sf::Vector2f{ (float) eventType.x, (float) eventType.y };
        };

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                const sf::Vector2f mouse = getMouseFromEvent(event.mouseButton);

                auto inverseMine = board.getMineTransform(getBoardTransform()).getInverse();
                sf::Vector2i mineIndex = static_cast<sf::Vector2i>(inverseMine.transformPoint(mouse));

            }

        }

        // RENDER HERE

        window.clear(sf::Color::White);

        drawBoard();
        drawMenu();

        window.display();
    }

};

int main(void)
{

    /*
    -std=c++17 -lsfml-system -lsfml-window -lsfml-graphics  
    */

    Minesweeper game {
        600, 600,
        30, 30
    };

    std::cout << "Running\n";
    while (game.isRunning()) {
        game.execute();
    }
    std::cout << "Closing\n";

    return 0;
}