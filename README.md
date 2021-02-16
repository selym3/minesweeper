# minesweeper

Classic minesweeper implementation in C++

## issues

* there is an issue with freeing and malloc at the beginning / end of execution. There are no manual frees in the code, so the program needs to be debugged

## todo

* there is no score system and the program still uses cout for feedback
* there should be an abstraction for function gettig which move to play, rather than always using mouse input
