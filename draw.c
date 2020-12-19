#include "draw.h"
#include "board.h"

void start_ncurses(){
    initscr();
    start_color();
    curs_set(1);
    timeout(0);
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void stop_ncurses(){
    clear();
    refresh();
    endwin();
}

void drawBoard(board *b){
    size_t scr_w, scr_h;
    getmaxyx(stdscr, scr_h, scr_w);
    size_t xoff = (scr_w - b->w) / 2;
    size_t yoff = (scr_h - b->h) / 2;
    size_t idx;
    for (size_t i = 0; i < b->h; i++){
        move(i+yoff, xoff);
        for(size_t j = 0; j < b->w; j++){
            idx = i * b->w + j;
            switch (b->cover[idx]){
                case CELL_COVER:
                    addch('#');
                    break;
                case CELL_FLAG:
                    addch('F');
                    break;
                case CELL_EXPLODE:
                    addch('X');
                    break;
                case CELL_CLEAR:
                    if (b->hints[idx]) addch('0'+b->hints[idx]);
                    else addch('.');
                    break;
            }
        }
    }
    refresh();
}

void play(board* b){
    int c, cury, curx;
    size_t scr_w, scr_h;
    getmaxyx(stdscr, scr_h, scr_w);
    size_t xoff = (scr_w - b->w) / 2;
    size_t yoff = (scr_h - b->h) / 2;
    cury = yoff;
    curx = xoff;
    int result = ACTION_SAFE;
    while ((c = getch()) != 'q'){
        switch (c){
            case KEY_DOWN:
                if (cury-yoff < b->h - 1) cury++;
                break;
            case KEY_UP:
                if (cury-yoff > 0) cury--;
                break;
            case KEY_LEFT:
                if (curx-xoff > 0) curx--;
                break;
            case KEY_RIGHT:
                if (curx-xoff < b->w - 1) curx++;
                break;
            case 'f':
                flagCell(b, curx-xoff, cury-yoff);
                drawBoard(b);
                break;
            case ' ':
                clearCell(b, curx-xoff, cury-yoff);
                drawBoard(b);
                break;
        }
        move(cury, curx);
    }
}
