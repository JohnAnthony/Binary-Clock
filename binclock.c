#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define BINLENGTH 8

static int rows, cols;
static int mask;

void drawtime(void);
void init(void);
void printbin(int value);

void
drawtime(void) {
    struct tm *timeset;
    time_t unixtime;
    int *timepart[3];
    int currow, curcol;
    int i;

    getmaxyx(stdscr, rows, cols);
    unixtime = time(NULL);
    timeset = localtime(&unixtime);

    timepart[0] = &timeset->tm_hour;
    timepart[1] = &timeset->tm_min;
    timepart[2] = &timeset->tm_sec;

    currow = (rows / 2) - 2;
    curcol = (cols / 2) - (BINLENGTH / 2);

    for (i = 0; i < 3; ++i, currow += 2) {
        move(currow, curcol);
        printbin(*timepart[i]);
    }
}

void
init(void) {
    initscr();
    raw();
    noecho();

    mask = 1 << (BINLENGTH - 1);
}

void
printbin(int value) {
    int i;

    for (i = 0; i < BINLENGTH; ++i) {
        if (value & mask)
            printw("1");
        else
            printw("0");
        value = value << 1;
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
