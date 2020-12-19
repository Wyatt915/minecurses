#include "draw.h"
#include "board.h"

void start_ncurses(){
    initscr();
    start_color();
    curs_set(1);
    timeout(0);

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_RED, COLOR_WHITE);
    init_pair(10, COLOR_RED, COLOR_WHITE);
    init_pair(11, COLOR_BLACK, COLOR_RED);

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
                    addch(ACS_CKBOARD);
                    break;
                case CELL_FLAG:
                    attron(COLOR_PAIR(10));
                    addch('F');
                    attroff(COLOR_PAIR(10));
                    break;
                case CELL_EXPLODE:
                    attron(COLOR_PAIR(11));
                    addch('X');
                    attroff(COLOR_PAIR(11));
                    break;
                case CELL_CLEAR: ;
                    int hint = b->hints[idx];
                    if (hint) {
                        attron(COLOR_PAIR(hint));
                        addch('0' + hint);
                        attroff(COLOR_PAIR(hint));
                    }
                    else addch(' ');
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
