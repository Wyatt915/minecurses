#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <stdlib.h>

#include "board.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    board *theBoard = createBoard(30, 20, 7*30*20/20);
    printBoard(theBoard);
    freeBoard(theBoard);
    return 0;
}
