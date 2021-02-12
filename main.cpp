#include <iostream>

// -lsfml-system -lsfml-window -lsfml-graphics

#include <SFML/Graphics.hpp>

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
        // TEMPORARY CODE: MOVE THIS INTO A FUNCTION
        auto top = states.transform.transformPoint({0,0});
        auto bottom = states.transform.transformPoint({1,1});

        auto size = bottom - top;

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

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // Rectangle boundary
        auto top = states.transform.transformPoint({0,0});
        auto bottom = states.transform.transformPoint({1,1});

        auto size = bottom - top;

        // Create a drawable field
        sf::RectangleShape field {};
        field.setPosition(top);
        field.setSize(size);
        field.setFillColor(sf::Color(155, 155, 155));

        // Draw the field
        target.draw(field);

        sf::Transform mineTransform { };
        mineTransform.translate(top);

        mineTransform.scale({ size.x / (float) cols, size.y / (float) rows });

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                auto mine = get(x, y);


                // mineTransform.translate(x, y);

                target.draw(*mine, sf::Transform(mineTransform).translate(x , y));
            }   
        }
    }

};

////

class Minesweeper 
{

    sf::RenderWindow window;
    std::size_t width, height; // <-- for convenience

    Minefield board;

    void centerWindow()
    {
        auto desktop = sf::VideoMode::getDesktopMode();
        window.setPosition(sf::Vector2i(
            desktop.width / 2 - window.getSize().x / 2,
            desktop.height / 2 - window.getSize().y / 2
        ));
    }

    void drawBoard()
    {
        // Transform for the board
        sf::Transform transform {};
        float top = 0.25;
        transform.translate({ 0 * 1.0f, height * top });
        transform.scale({ width * 1.0f , height * (1 - top) });

        // Draw the board with the transform
        window.draw(board, transform);
    }

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

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
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