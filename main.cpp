#include <iostream>
#include <cstring>

#include "./headers/minesweeper.hpp"

/**
 * This struct describes how to create a 
 * minefield game board.
 */
struct Difficulty
{

    const std::size_t cols, rows, bombs;

    Difficulty(std::size_t cols, std::size_t rows, std::size_t bombs) :
        cols { cols }, rows { rows }, bombs { bombs }
    {
    }

    const static Difficulty EASY;
    const static Difficulty INTERMIEDIATE;
    const static Difficulty EXPERT;

    const static Difficulty DEFAULT;
};

// Prototype for a parsing function
Difficulty getDifficulty(int argc, char ** argv);

/////////
int main(int argc, char ** argv)
{

    /*
    -std=c++17 -lsfml-system -lsfml-window -lsfml-graphics  
    */

    auto [ cols, rows, bombs ] = getDifficulty(argc, argv); 

    Minesweeper game{
        900, 900,
        cols, rows, bombs
    };

    std::cout << "Running\n";
    while (game.isPlaying()) {
        game.execute();
    }
    std::cout << "Closing\n";

    return 0;
}
/////////

/**
 * These are the preset constants for minefield difficulty
 */
const Difficulty Difficulty::EASY = Difficulty(8, 8, 10);
const Difficulty Difficulty::INTERMIEDIATE = Difficulty(16, 16, 40);
const Difficulty Difficulty::EXPERT = Difficulty(16, 30, 90);

const Difficulty Difficulty::DEFAULT = Difficulty::INTERMIEDIATE;

/**
 * This function parses the command line arguments and 
 * decides on a difficulty.
 */
Difficulty getDifficulty(int argc, char ** argv)
{
    argc--;
    
    if (argc == 0) {
        return Difficulty::DEFAULT;
    }

    else if (argc == 1) {
        if (std::strcmp("easy", argv[1]) == 0)
            return Difficulty::EASY;
        else if (std::strcmp("intermediate", argv[1]) == 0)
            return Difficulty::INTERMIEDIATE;
        else if (std::strcmp("expert", argv[1]) == 0)
            return Difficulty::EXPERT;
        else
            return Difficulty::DEFAULT;
    }
     
    else if (argc == 3) {
        std::size_t cols = std::atoi(argv[1]),
                    rows = std::atoi(argv[2]),
                    bombs = std::atoi(argv[3]);

        return Difficulty(cols, rows, bombs);
    }

    throw std::runtime_error("Could not parse difficulty from command line arguments");
}
////////