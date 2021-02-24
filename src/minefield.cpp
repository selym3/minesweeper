#include "../headers/minefield.hpp"
#include "../headers/utils/random_engine.hpp"
#include "../headers/utils/utils.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>

void Minefield::shuffleMines()
{
    std::shuffle(
        mines.begin(), 
        mines.end(), 
        Random::getEngine()
    );
}

void Minefield::setMines(/* int mines */)
{
    for (int i = 0; i < mines.size(); ++i)
        mines[i].bomb = (i < bombs);
}

Minefield::Minefield(std::size_t cols, std::size_t rows, std::size_t bombs) : 
    cols { cols },
    rows { rows }, 
    bombs { bombs > cols * rows ? cols * rows : bombs }
{
    auto size = cols * rows;

    mines.resize(size);

    resetAll();
}

/***********
 * GETTERS *
 ***********/

bool Minefield::inBounds(int x, int y) const
{
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

const Mine& Minefield::get(int x, int y) const
{
    return mines[y * cols + x];
}

Mine& Minefield::get(int x, int y) 
{
    return mines[y * cols + x];
}

int Minefield::getNeighbors(int x, int y) const
{
    int neighbors = 0;

    int xi, yj;

    for (int j = -1; j <= 1; ++j) {
        for (int i = -1; i <= 1; ++i) {
            xi = x + i; yj = y + j;

            if (inBounds(xi, yj) && ((i!=0) || (j!=0)))
                neighbors += get(xi, yj).bomb;
        }
    }

    return neighbors;
}

/***********
 * SETTERS *
 ***********/

bool Minefield::reveal(int x, int y)
{
    if (!inBounds(x, y))
        return false;

    Mine &mine = get(x, y);

    if (!mine.discovered() && !mine.bomb && !mine.flagged()) {
        mine.neighbors = getNeighbors(x, y);
        mine.state = Mine::State::Discovered;

        if (mine.neighbors == 0) {
            for (int j = -1; j <= 1; ++j) {
                for (int i = -1; i <= 1; ++i) {
                    if (i != 0 || j != 0)
                        reveal(x + i, y + j);
                }
            }
        }
    }

    return !mine.flagged() && mine.bomb;
}

void Minefield::revealAll()
{
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            Mine& mine = get(x, y);

            mine.state = Mine::State::Discovered;
            mine.neighbors = getNeighbors(x, y);
        }
    }

    // for (auto& mine : mines)
        // mine.state = Mine::State::Discovered;
}

void Minefield::resetAll()
{
    for (auto& mine : mines)
        mine.reset();

    setMines();
    shuffleMines();
}

bool Minefield::flag(int x, int y)
{
    Mine& mine = get(x, y);

    if (mine.discovered()) 
        return true;

    mine.state = mine.flagged() ?
        Mine::State::Default :
        Mine::State::Flagged;

    return false;
}

/*************
 * RENDERING *
 ************/

sf::Transform Minefield::getMineTransform(const sf::Transform& transform) const
{
    auto [ top, bottom, size ] = Utils::getRectangle(transform);

    return getMineTransform(top, size);
}

sf::Transform Minefield::getMineTransform(const sf::Vector2f& top, const sf::Vector2f& size) const
{
    sf::Transform mineTransform {};
    mineTransform.translate(top);
    mineTransform.scale({ size.x / (float) cols, size.y / (float) rows });

    return mineTransform;
}

void Minefield::draw(sf::RenderTarget& target, sf::RenderStates states) const
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