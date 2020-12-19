#ifndef DRAW_H
#define DRAW_H

#include "board.h"
#include <ncurses.h>

void start_ncurses();
void stop_ncurses();
void drawBoard(board*);
void play(board*);

#endif // DRAW_H
