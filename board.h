#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//Gives the ability to treat a 1d array as a 2d array
//#define IDX(GRID, R, C) ((GRID->data)[(R) * (GRID->cols) + (C)])

#define CELL_COVER 0
#define CELL_FLAG  1
#define CELL_CLEAR 2
#define CELL_EXPLODE 3

#define ACTION_SAFE 0
#define ACTION_DEAD 1

typedef struct {
    size_t w;
    size_t h;
    uint8_t *mines; // Array of size w*h. Marks mine locations.
    uint8_t *hints; // Array of size w*h. Contains all digits.
    uint8_t *cover; // Array of size w*h. Determines the state of each cell.
    size_t numMines;
    size_t maxMines;
    bool isFirstMove;
    bool isDead;
} board;

board * createBoard(size_t w, size_t h, size_t maxMines);
void freeBoard(board*);

int clearCell(board* b, int x, int y);
int flagCell(board* b, int x, int y);
int unflagCell(board* b, int x, int y);
bool gameOver(board*);
void printBoard(board*);


#endif //BOARD_H
