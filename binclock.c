#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define BINLENGTH 8

/* Globals */
static int rows, cols;
static int drawrow, drawcol;
static int mask;
static char *bracestrings[2];
static int colourset[3] = { 0, 0, 0 };

/* Globals that are expected to be set via flags */
static int BRACES_ON = 1;
static int COLOURS_ON = 0;
static char* ON_OFF_STRINGS[2] = { "0", "1" };

/* Predecs */
void drawtime(void);
void init(void);
void printbin(int value, int color);

/* Function definitions */
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
        printbin(*timepart[i], colourset[i]);
    }
}

void
init(void) {
    initscr();
    start_color();
    raw();
    noecho();
    getmaxyx(stdscr, rows, cols);

    mask = 1 << (BINLENGTH - 1);
    drawcol = (cols / 2) - (BINLENGTH / 2);

    /* If we want COLOURS_ON... */
    if (COLOURS_ON)
        COLOURS_ON = has_colors(); /* Set that flag to either yes or no depending on availability */
    if (COLOURS_ON) {
        init_pair(1,  COLOR_RED,     COLOR_BLACK);
        init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
        init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
        init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
        init_pair(6,  COLOR_CYAN,    COLOR_BLACK);

        colourset[0] = 1;
        colourset[1] = 2;
        colourset[2] = 3;
    }

    /* Handle the config of the braces */
    if (BRACES_ON){
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
printbin(int value, int colour) {
    int i;
    for (i = 0; i < BINLENGTH; ++i, value = value << 1) {
        printw(bracestrings[0]);
        color_set(colour, NULL);
        if (value & mask)
            printw(ON_OFF_STRINGS[1]);
        else
            printw(ON_OFF_STRINGS[0]);
        color_set(0, NULL);
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
