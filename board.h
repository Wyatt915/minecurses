#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

//Gives the ability to treat a 1d array as a 2d array
//#define IDX(GRID, R, C) ((GRID->data)[(R) * (GRID->cols) + (C)])

typedef struct {
    size_t w;
    size_t h;
    uint8_t *mines; // Array of size w*h. Marks mine locations.
    uint8_t *hints; // Array of size w*h. Contains all digits.
    size_t numMines;
} board;

board * createBoard(size_t w, size_t h, size_t maxMines);
void freeBoard(board*);
void printBoard(board*);
#endif //BOARD_H
