#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define BINLENGTH 8

static int rows, cols;
static int drawrow, drawcol;
static int mask;

static int braces = 1;

static char *bracestrings[2];

void drawtime(void);
void init(void);
void printbin(int value);

void
drawtime(void) {
    struct tm *timeset;
    time_t unixtime;
    int *timepart[3];
    int i;

    getmaxyx(stdscr, rows, cols);
    unixtime = time(NULL);
    timeset = localtime(&unixtime);

    timepart[0] = &timeset->tm_hour;
    timepart[1] = &timeset->tm_min;
    timepart[2] = &timeset->tm_sec;

    drawrow = (rows / 2) - 2;

    for (i = 0; i < 3; ++i, drawrow += 2) {
        move(drawrow, drawcol);
        printbin(*timepart[i]);
    }
}

void
init(void) {
    initscr();
    raw();
    noecho();
    getmaxyx(stdscr, rows, cols);

    mask = 1 << (BINLENGTH - 1);
    drawcol = (cols / 2) - (BINLENGTH / 2);

    /* Handle the config of the braces */
    if (braces){
        drawcol -= BINLENGTH;
        bracestrings[0] = "[";
        bracestrings[1] = "]";
    }
    else {
        bracestrings[0] = "";
        bracestrings[1] = "";
    }
}

void
printbin(int value) {
    int i;
    for (i = 0; i < BINLENGTH; ++i, value = value << 1) {
        printw(bracestrings[0]);
        if (value & mask)
            printw("1");
        else
            printw("0");
        printw(bracestrings[1]);
    }
}

int main(void)
{
    init();
    while (1) {
        drawtime();
        refresh();
        sleep(1);
    }
    endwin();

    return 0;
}
