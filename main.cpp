#include <iostream>
#include <SFML/Graphics.hpp>

#include <sstream>
#include <unordered_map>


#include <optional>
#include <vector>
#include <cstdlib>

namespace FontLoader
{

    using FontLoad = std::optional<std::reference_wrapper<const sf::Font>>;

    static FontLoad load(const std::string& path)
    {
        // LOOKUP TABLE
        static std::unordered_map<std::string, sf::Font> LOOKUP_TABLE;

        // ALGORITHM

        auto fontEntry = LOOKUP_TABLE.find(path);
        bool contains = fontEntry != LOOKUP_TABLE.end();

        if (contains) {
            return std::ref(fontEntry->second);
        } else {

            sf::Font font;

            bool loaded = font.loadFromFile(path);

            if (loaded) {
                auto entry = LOOKUP_TABLE.insert({ path, font });

                return std::ref(entry.first->second);

            } else {
                return std::nullopt;
            }

        }

    }

};

sf::Text getText(
    const std::string &txt,
    const sf::Font &font,
    unsigned int txtSize = 12,
    sf::Color color = sf::Color::Black,
    sf::Vector2f position = sf::Vector2f{0, 0},
    sf::Text::Style style = sf::Text::Style::Regular)
{
    sf::Text text;

    text.setCharacterSize(txtSize);

    // text.setScale((float) txtSize, (float) txtSize);
    text.setPosition(position);

    text.setFont(font);
    text.setFillColor(color);
    text.setStyle(style);

    text.setString(txt);
    return text;
}

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

    enum State {
        Default,
        Discovered,
        Flagged
    };

    bool bomb;
    State state = State::Default;

    bool discovered() const { return state == State::Discovered; }
    bool flagged() const { return state == State::Flagged; }

    unsigned int neighbors;

    // Render information

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        auto [ top, bottom, size ] = getRectangle(states.transform);

        // Create a drawable field
        sf::RectangleShape field {};
        field.setPosition(top);
        field.setSize(size);
        field.setFillColor(sf::Color{ 155, 155, 155 });

        field.setOutlineColor(
            discovered() || flagged() ?
                sf::Color{ 200, 200, 200 } : 
                sf::Color::Black
        );
        field.setOutlineThickness(
            discovered() ?
                2 :
                0.5f
        );
        
        target.draw(field);

        // Draw text if necessary
        auto fontLoad = FontLoader::load("resources/source-code.ttf");

        if (fontLoad.has_value()) {

            auto [ text, color ] = getInfo();

            top.y -= size.y * 0.2;

            if (!text.empty()) {
                target.draw(getText(
                    text,
                    *fontLoad,
                    size.y,
                    color,
                    top
                ));
            }
        }

    }

private:

    inline std::pair<std::string, sf::Color> getInfo() const
    {
        std::string text;
        sf::Color color { sf::Color::White };

        if (discovered()) {

            if (bomb) {
                text = "B";
                color = sf::Color::Red;
            }
            else if (neighbors > 0) {
                text = std::to_string(neighbors);
            }
            
        } else if (flagged()) {
            text = "F";
            color = sf::Color::Blue;
        }

        return { text, color };
    }

};

//

class Minefield : public sf::Drawable 
{

    std::vector<Mine> mines;

    void fillMines()
    {
        auto randomMine = [this]() -> Mine * {
            return &mines[rand() % cols + (rand() % rows) * cols];
        };

        for (int i = 0; i < bombs; ++i) {
            Mine * mine = randomMine();

            while (mine->bomb)
                mine = randomMine();

            mine->bomb = true;

        }
    }

public:
    const std::size_t cols, rows, bombs;

    Minefield(std::size_t cols, std::size_t rows, std::size_t bombs)
        : cols { cols }, rows { rows }, bombs { bombs }
    {
        auto size = cols * rows;

        mines.resize(size);
        fillMines();
    }

    /**
     * Getters
     */

    bool inBounds(int x, int y) const
    {
        return x >= 0 && x < cols && y >= 0 && y < rows;
    }

    const Mine& get(int x, int y) const
    {
        return mines[y * cols + x];
    }

    Mine& get(int x, int y) 
    {
        return mines[y * cols + x];
    }

    int getNeighbors(int x, int y) const
    {
        int neighbors = 0;

        int xi, yj;

        for (int j = -1; j <= 1; ++j) {
            for (int i = -1; i <= 1; ++i) {
                xi = x + i; yj = y + j;

                if (inBounds(xi, yj) && ((i!=0) && (j!=0)))
                    neighbors += get(xi, yj).bomb;
            }
        }

        return neighbors;
    }

    /**
     * Setters
     */

    bool reveal(int x, int y) 
    {
        // NOTE: Doesn't care if the mine is already revealed
        Mine& mine = get(x, y);

        if (!mine.flagged()) {
            mine.neighbors = getNeighbors(x, y);
            mine.state = Mine::State::Discovered;
        }

        return mine.bomb;
    }

    bool flag(int x, int y)
    {
        Mine& mine = get(x, y);

        if (mine.discovered()) 
            return true;

        mine.state = mine.flagged() ?
            Mine::State::Default :
            Mine::State::Flagged;

        return false;
    }

    /**
     * Rendering
     */

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

                target.draw(mine, sf::Transform(mineTransform).translate(x, y));
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

    bool started = false, 
            lost = false;
    sf::Clock timer;

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

    float top = 0.10;

    sf::Transform getBoardTransform() const
    {
        sf::Transform transform {};
        transform.translate(0 * 1.0f, height * top);
        transform.scale(width * 1.0f, height * (1 - top));

        return transform;
    }

    void drawBoard()
    {
        // Transform for the board
        sf::Transform transform = getBoardTransform();

        window.draw(board, transform);
    }

    // Menu utils

    void drawMenu()
    {

    }

    // Event utils

    sf::Vector2i getMineIndex(const sf::Vector2f in) const
    {
        sf::Transform tfm = board.getMineTransform(getBoardTransform());
        tfm = tfm.getInverse();

        return static_cast<sf::Vector2i>(
            tfm.transformPoint(in)
        );
    }

    inline sf::Vector2i getMineIndex(const sf::Event::MouseButtonEvent& event) const
    {
        return getMineIndex({
            (float) event.x,
            (float) event.y
        });
    }

    void handleMouse(const sf::Event &event)
    {
        auto [x, y] = getMineIndex(event.mouseButton);

        if (!board.inBounds(x, y))
            std::cout << "Mine is out of bounds " << x << ", " << y << "!\n";

        else {

            if (event.mouseButton.button == sf::Mouse::Left) {
                this->lost = board.reveal(x, y);
            }

            else if (event.mouseButton.button == sf::Mouse::Right) {
                bool flagged = board.flag(x, y);
            }

        }
    }

    inline bool shouldRun() const
    {
        return started && !lost;
    }

public:

    Minesweeper(
        std::size_t width, std::size_t height, 
        std::size_t cols,  std::size_t rows,
        std::size_t bombs
    ) :
        window { 
            sf::VideoMode(width, height), 
            "Minesweeper", 
            sf::Style::None 
        },
        width { width }, height { height },
        board { cols , rows, bombs }
    {
        centerWindow();
    }

    bool isRunning() const
    {
        return window.isOpen();
    }

    void execute()
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
                        this->started = true;
                        timer.restart();
                    }
                }
            }
            
            // MOUSE FUNCTIONALITY

            else if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "Mouse button registered\n!";

                if (shouldRun())
                    handleMouse(event);
            }

        }

        if (lost) {
            std::cout << "Clicked on a mine!\n";
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
    clang++ main.cpp -o main -std=c++17 -lsfml-system -lsfml-window -lsfml-graphics

    -std=c++17 -lsfml-system -lsfml-window -lsfml-graphics  
    */

    Minesweeper game {
        600, 600,
        30, 30, 
        100
    };

    std::cout << "Running\n";
    while (game.isRunning()) {
        game.execute();
    }
    std::cout << "Closing\n";

    return 0;
}