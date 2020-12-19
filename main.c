#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <stdlib.h>

#include "board.h"
#include "draw.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    board *theBoard = createBoard(30, 20, 7*30*20/20);
    start_ncurses();
    drawBoard(theBoard);
    play(theBoard);
    stop_ncurses();
    freeBoard(theBoard);
    return 0;
}
