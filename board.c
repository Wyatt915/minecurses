#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "board.h"

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
            field[i * w + j] = count[i*w+j] >= 5 || count[i*w+j] == 0 ? 1 : 0;
        }
    }
    free(count);
}

void addMines(board *b, size_t maxMines){
    uint8_t * mines = b -> mines;
    size_t w = b -> w;
    size_t h = b -> h;
    size_t cells = w*h;
    int mineProb = ((float) maxMines / (float) cells) * 1000;

    for (size_t i = 0; i < cells; i++){
        if (rand() % 1000 < mineProb){
            mines[i] = 1;
        }
    }

    for (size_t i = 0; i < cells; i++){
        b -> numMines += mines[i];
    }

    printBoard(b);
    automata(w, h, b -> mines);
    b -> numMines = 0;
    for (size_t i = 0; i < cells; i++){
        b -> numMines += mines[i];
    }

}


board * createBoard(size_t w, size_t h, size_t maxMines){
    board * theBoard = calloc(1, sizeof(board));
    theBoard -> w = w;
    theBoard -> h = h;
    theBoard -> mines = calloc(w*h, sizeof(uint8_t));
    theBoard -> hints = calloc(w*h, sizeof(uint8_t));
    addMines(theBoard, maxMines);
    countNeighbors(w, h, theBoard -> mines, theBoard -> hints);
    return theBoard;
}

void freeBoard(board* b){
    free(b -> mines);
    free(b -> hints);
    free(b);
}

void printBoard(board* b){
    size_t w = b -> w;
    size_t h = b -> h;
    uint8_t * mines = b -> mines;
    for (size_t i = 0; i < h; i++){
        for (size_t j = 0; j < w; j++){
            mines[i * w + j] ? putchar('#') : putchar('.');
        }
        putchar('\n');
    }
    printf("Total mines: %d\n", b -> numMines);
}

