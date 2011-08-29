/* ============================================================================================ */
/* This software is created by LAMMJohnson and comes with no warranty of any kind.              */
/*                                                                                              */
/* If you like this software and would like to contribute to its continued improvement          */
/* then please feel free to submit bug reports here: www.github.com/LAMMJohnson                 */
/*                                                                                              */
/* This program is licensed under the GPLv3 and in support of Free and Open Source              */
/* Software in general. The full license can be found at http://www.gnu.org/licenses/gpl.html   */
/* ============================================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <signal.h>
#include <string.h>

#define NUM_THEMES 4

/* Globals */
static int rows, cols;
static int drawrow, drawcol;
static int mask;
static char *bracestrings[2];
static int colourset[3] = { 0, 0, 0 };
static int running = 1;
static int THEME_NUMBER = 0;
static int BINLENGTH = 8;
static int BRACES_ON = 1;
static int COLOURS_ON = 1;
static int LABELS_ON = 1;
static char *labels[3] = {  "  Hours: ",
                            "Minutes: ",
                            "Seconds: " };
static char *ON_OFF_STRINGS[NUM_THEMES][2] = {  { " ", "*" },
                                                { "0", "1" },
                                                { " ", "+" },
                                                { " ", "X" } };
/* Predecs */
static void calc_draw_pos(void);
static void drawtime(void);
static void run_off(int sig);
static void handle_args(int argc, char** argv);
static void handle_input(void);
static void init(void);
static void printbin(int value, int color);
static void set_theme(int i);
static void usage(int err);

/* Function definitions */
static void
calc_draw_pos(void) {
    drawcol = (cols / 2) - ((BINLENGTH * strlen(ON_OFF_STRINGS[THEME_NUMBER][0])) / 2);

    if(BRACES_ON)
        drawcol -= (strlen(bracestrings[0]) + strlen(bracestrings[1])) * (BINLENGTH / 2);
    /* Handle the config of labels on/off */
    if (LABELS_ON)
        drawcol -= strlen(labels[0]);
}

static void
drawtime(void) {
    struct tm *timeset;
    time_t unixtime;
    int *timepart[3];
    int oldcols, oldrows;
    int i;

    oldcols = cols;
    oldrows = rows;
    getmaxyx(stdscr, rows, cols);
    if (oldcols != cols || oldrows != rows) {
        clear();
        calc_draw_pos();
    }
    else
        drawrow = (rows / 2) - 2;

    unixtime = time(NULL);
    timeset = localtime(&unixtime);

    timepart[0] = &timeset->tm_hour;
    timepart[1] = &timeset->tm_min;
    timepart[2] = &timeset->tm_sec;

    for (i = 0; i < 3; ++i, drawrow += 2) {
        move(drawrow, drawcol);
        if (LABELS_ON)
            printw(labels[i]);
        printbin(*timepart[i], colourset[i]);
    }
}

static void
handle_args(int argc, char** argv) {
    int i;

    for (i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-h"))
            usage(0);
        else if (!strcmp(argv[i], "-l"))
            LABELS_ON = 1;
        else if (!strcmp(argv[i], "-nl"))
            LABELS_ON = 0;
        else if (!strcmp(argv[i], "-c"))
            COLOURS_ON = 1;
        else if (!strcmp(argv[i], "-nc"))
            COLOURS_ON = 0;
        else if (!strcmp(argv[i], "-b"))
            BRACES_ON = 1;
        else if (!strcmp(argv[i], "-nb"))
            BRACES_ON = 0;
        /* Theme selection */
        else if (!strcmp(argv[i], "-t")) {
            if (++i < argc)
                set_theme(atoi(argv[i]));
            else {
                puts("Theme option needs an argument.");
                usage(2);
            }
        }
        /* if we get to here we have an unrecognised flag being passed */
        else {
            printf("Unrecognised option: '%s'\n", argv[i]);
            usage(1);
        }
    }
}

static void
handle_input(void) {
}

static void
run_off(int sig) {
    running = 0;
}

static void
init(void) {
    initscr();
    clear();
    start_color();
    raw();
    noecho();
    nonl();
    keypad(stdscr, true);
    cbreak();
    curs_set(0);
    getmaxyx(stdscr, rows, cols);

    /* Handle signals correctly */
    signal(SIGTERM, run_off);

    mask = 1 << (BINLENGTH - 1);

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
        bracestrings[0] = "[";
        bracestrings[1] = "]";
    }
    else {
        bracestrings[0] = "";
        bracestrings[1] = " ";
    }

    calc_draw_pos();
}

static void
printbin(int value, int colour) {
    int i;
    for (i = 0; i < BINLENGTH; ++i, value = value << 1) {
        printw(bracestrings[0]);
        color_set(colour, NULL);
        if (value & mask)
            printw(ON_OFF_STRINGS[THEME_NUMBER][1]);
        else
            printw(ON_OFF_STRINGS[THEME_NUMBER][0]);
        color_set(0, NULL);
        printw(bracestrings[1]);
    }
}

static void
set_theme(int i) {
    i--;

    if (i < 0 || i >= NUM_THEMES) {
        puts("Theme selected is not within range!");
        usage(3);
    }

    THEME_NUMBER = i;
}

static void
usage(int err) {
    endwin();
    puts("Usage flags:");
    puts("  -h          Display (this) help text");
    puts("  -c          Colour on (default)");
    puts("  -nc         Colour off");
    puts("  -l          Labels on (default)");
    puts("  -nl         Labels off");
    puts("  -b          Braces on (default)");
    puts("  -nb         Braces off");
    puts("  -t #        Set theme # (0-1)");
    exit(err);
}

int main(int argc, char** argv)
{
    handle_args(argc, argv);
    init();
    while (running) {
        drawtime();
        refresh();
        handle_input();
        sleep(1);
    }
    endwin();

    return 0;
}
