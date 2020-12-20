#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "board.h"

//-----------------------------------------[Board Creation]-----------------------------------------

void countNeighbors(size_t w, size_t h, uint8_t * field, uint8_t * count){
    memset(count, 0x0, w*h*sizeof(uint8_t));
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            for (int y = i-1; y <= i+1; y++){
                if (y < 0)  continue;
                if (y >= h) continue;
                for (int x = j-1; x <= j+1; x++){
                    if (x < 0)  continue;
                    if (x >= w) continue;
                    count[i * w + j] += field[y * w + x];
                }
            }
        }
    }
}

void automata(size_t w, size_t h, uint8_t * field){
    uint8_t * count = malloc(w * h * sizeof(uint8_t));
    countNeighbors(w, h, field, count);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            field[i*w+j] = count[i*w+j] >= 5 || count[i*w+j] == 0 ? 1 : 0;
        }
    }
    free(count);
}

void addMines(board *b, size_t maxMines){
    uint8_t * mines = b -> mines;
    size_t w = b -> w;
    size_t h = b -> h;
    size_t cells = w*h;
    b -> numMines = 0;
    int mineProb = ((float) maxMines / (float) cells) * 1000;

    for (size_t i = 0; i < cells; i++){
        if (rand() % 1000 < mineProb){
            mines[i] = 1;
        }
    }

    automata(w, h, b -> mines);

    b -> numMines = 0;
    for (size_t i = 0; i < cells; i++){
        b -> numMines += mines[i];
    }

}


void addHints(board *b){
    countNeighbors(b->w, b->h, b->mines, b->hints);
    for (size_t i = 0; i < b->w * b->h; i++){
        if (b->mines[i]) b->hints[i] = 0;
    }
}


/**
 * Creates the board as soon as the player selects the first cell. The first
 * cell will never be a mine.
 * @param w The width of the board
 * @param h The height of the board
 * @param maxMines the approximate number of mines to be placed
 */
board * createBoard(size_t w, size_t h, size_t maxMines){
    board * theBoard = calloc(1, sizeof(board));
    theBoard -> w = w;
    theBoard -> h = h;
    theBoard -> isFirstMove = true;
    theBoard -> isDead = false;
    theBoard -> maxMines = maxMines;
    theBoard -> mines = calloc(w*h, sizeof(uint8_t));
    theBoard -> hints = calloc(w*h, sizeof(uint8_t));
    theBoard -> cover = calloc(w*h, sizeof(uint8_t));
    return theBoard;
}

void freeBoard(board* b){
    free(b -> mines);
    free(b -> hints);
    free(b);
}

//---------------------------------------[Player Interaction]---------------------------------------


/**
 * Implements a 4-way flood fill.
 * */
void floodFill(board *b, int x, int y){
    //return if we are out of bounds.
    if (x < 0 || y < 0 || x >= b->w || y >= b->h) return;

    size_t idx = y * b->w + x;
    // Check if we've been here before.
    if (b->cover[idx] == CELL_CLEAR) return;
    // Stop the flood fill once a mine is found
    if (b->mines[idx]) return;
    // No that we're here for the first time, we need to clear the cell.
    b->cover[idx] = CELL_CLEAR;
    // Otherwise, continue flood
    floodFill(b, x-1, y);
    floodFill(b, x+1, y);
    floodFill(b, x, y-1);
    floodFill(b, x, y+1);
}

int clearCell(board *b, int x, int y){
    size_t idx = y * b->w + x;
    // Check if this is the first move
    if (b->isFirstMove){
        b->isFirstMove = false;
        // If there is a mine at (x, y), scrap the whole board and try again.
        do {
            memset(b->mines, 0x0, b->w * b->h * sizeof(uint8_t));
            addMines(b, b->maxMines);
        } while (b->mines[idx]);
        addHints(b);
    }
    // if the cell is flagged, nothing happens
    if (b->cover[idx] == CELL_FLAG) return ACTION_SAFE;
    if (b->mines[idx]){
        b->cover[idx] = CELL_EXPLODE;
        b->isDead = true;
        return ACTION_DEAD;
    }
    // If the uncoverd area is not adjacent to a mine, uncover all adjacent
    // areas bounded by cells with hints
    if (!b->hints[idx]) floodFill(b, x, y);
    else b->cover[idx] = CELL_CLEAR;
    return ACTION_SAFE;
}

/**
 * Toggle whether or not a cell is flagged.
 */
int flagCell(board* b, int x, int y){
    size_t idx = y * b->w + x;
    // If the cell has been cleared, do nothind
    if (b->cover[idx] == CELL_CLEAR) return ACTION_SAFE;
    b->cover[idx] = b->cover[idx] == CELL_COVER ? CELL_FLAG : CELL_COVER;
}

bool gameOver(board* b){
    if (b->isFirstMove) return false;
    if (b->isDead) return true;
    // If every mine is flagged, we win!
    for (size_t i = 0; i < b->w * b->h; i++){
        if (b->mines[i] && !(b->cover[i] == CELL_FLAG)) return false;
    }
    return true;
}

void printBoard(board* b){
    size_t w = b -> w;
    size_t h = b -> h;
    uint8_t * mines = b -> mines;
    uint8_t * hints = b -> hints;
    for (size_t i = 0; i < h; i++){
        for (size_t j = 0; j < w; j++){
            mines[i * w + j] ? putchar('#') : putchar('.');
        }
        putchar(' '); putchar(' '); putchar(' '); putchar(' ');
        for (size_t j = 0; j < w; j++){
            hints[i*w+j] ? putchar(hints[i*w+j] + '0') : putchar('.');
        }
        putchar('\n');
    }
    printf("Total mines: %d\n", b -> numMines);
}

