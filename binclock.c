/*
 *   Copyright 2011, 2012 John Anthony and licensed under the GPLv3
 *   Copyright 2013 Cyphar (added interactive mode)
 *   See README.md and LICENSE files for more information
 */

#include <assert.h>
#include <getopt.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define LENGTH(x) (( sizeof(x) / sizeof(x[0]) ))

/* Enums */

typedef enum Colour {
	COLOUR_WHITE,
	COLOUR_RED,
	COLOUR_GREEN,
	COLOUR_YELLOW,
	COLOUR_BLUE,
	COLOUR_MAGENTA,
	COLOUR_CYAN,
	COLOUR_BLACK,
	COLOUR_LAST
} Colour;

/* Typedefs */

typedef struct ColourSet {
	Colour hour;
	Colour min;
	Colour sec;
} ColourSet;

typedef struct Theme {
	char left;
	char right;
	char off;
	char on;
} Theme;

typedef struct Config {
	Theme theme;
	int themeindex;
	ColourSet colourset;
	int colourindex;
} Config;

typedef struct State {
	int cols;
	int rows;
	int draw_col;
	int draw_row;
} State;

/* Globals */

static bool RUNNING;

static Theme themes[] = {
	{'[', ']', ' ', '*'},
	{'<', '>', '0', '1'},
	{'(', ')', ' ', '+'}
};

static ColourSet sets[] = {
	{COLOUR_RED, COLOUR_GREEN, COLOUR_YELLOW},
	{COLOUR_BLUE, COLOUR_MAGENTA, COLOUR_CYAN},
	{COLOUR_RED, COLOUR_WHITE, COLOUR_BLUE}
};

/* Function Predecs */

static void draw_time(Config *conf, State *s);
static void draw_line(int x, int y, Theme *t, Colour c, int value);
static Config* handle_args(int argc, char **argv);
static void handle_sigterm(int sig);
static State* init(Config *conf);
static void refresh_position(State *s);
static void usage_and_exit(int status);

/* Functions */

static void
draw_time(Config *conf, State *s) {
	struct tm *timeset;
	time_t unixtime;

	unixtime = time(NULL);
	timeset = localtime(&unixtime);

	refresh_position(s);
	draw_line(s->draw_col, s->draw_row + 0, &conf->theme,
		conf->colourset.hour, timeset->tm_hour);
	draw_line(s->draw_col, s->draw_row + 2, &conf->theme,
		conf->colourset.min,  timeset->tm_min);
	draw_line(s->draw_col, s->draw_row + 4, &conf->theme,
		conf->colourset.sec,  timeset->tm_sec);
}

static void
draw_line(int col, int row, Theme *t, Colour c, int value) {
	static const int binlength = 8;
	static const int mask = 1 << 7;
	char onoff;
	
	wmove(stdscr, row, col);
	for (int i = 0; i < binlength; ++i, value = value << 1) {
		if (value & mask)
			onoff = t->on;
		else
			onoff = t->off;

		wprintw(stdscr, "%c", t->left);
		color_set(c, NULL);
		wprintw(stdscr, "%c", onoff);
		color_set(0, NULL);
		wprintw(stdscr, "%c ", t->right);
	}
}

static Config*
handle_args(int argc, char **argv) {
	Config *conf;
	int i;
	int tmp;

	conf = malloc(sizeof(Config));
	assert(conf);

	/* Initialise defaults */
	conf->theme = themes[0];
	conf->themeindex = 0;
	conf->colourset = sets[0];
	conf->colourindex = 0;

	/* Actual parsing */
	while( (i = getopt(argc, argv, "ht:s:")) != -1 ) {
		switch(i) {
		case 'h':
			free(conf);
			usage_and_exit(EXIT_SUCCESS);
			break;
		case 't':
			tmp = atoi(optarg);
			assert(tmp >= 0 && tmp < LENGTH(themes));
			conf->theme = themes[tmp];
			conf->themeindex = tmp;
			break;
		case 's':
			tmp = atoi(optarg);
			assert(tmp >= 0 && tmp < LENGTH(sets));
			conf->colourset = sets[tmp];
			conf->colourindex = tmp;
			break;
		case '?':
		default:
			if (strchr("t", optopt)){
				fprintf(stderr, "Option -%c requires an argument\n",
					optopt);
			}
			else
				fprintf(stderr, "Unknown option: -%c\n", i);
			exit(EXIT_FAILURE);
		}
	}

	return conf;
}

static void
handle_sigterm(int sig) {
	RUNNING = false;
}

static State*
init(Config *conf) {
	State *s;

	assert( initscr() );
	clear();
	start_color();
	raw();
	noecho();
	nonl();
	keypad(stdscr, true);
	cbreak();
	curs_set(0);

	signal(SIGTERM, handle_sigterm);

	s = malloc(sizeof(State));
	assert(s);
	refresh_position(s);

	/* Colours */
	for (int i = 1; i < COLOUR_LAST; ++i)
		init_pair(i, i, COLOR_BLACK);

	/* TODO: Colour handling */
	return s;
}

static void
refresh_position(State *s) {
	static int output_length = 31;
	static int v_offset = 2;
	int oldcols, oldrows;

	oldrows = s->rows;
	oldcols = s->cols;
	getmaxyx(stdscr, s->rows, s->cols);

	if (oldcols == s->cols && oldrows == s->rows)
		return;
	
	erase();
	s->draw_col = s->cols / 2 - output_length / 2; 
	s->draw_row = s->rows / 2 - v_offset;
}

static void
usage_and_exit(int status) {
	fprintf(stderr,
"Please see the README or man page for usage instructions.\n\
\n\
  ::  man binclock\n\
  ::  cat README.md");
	exit(status);
}

int main(int argc, char **argv) {
	Config *conf;
	State *s;

	conf = handle_args(argc, argv);
	s = init(conf);

	timeout(1000);

	RUNNING = true;
	while (RUNNING) {
		refresh();
		draw_time(conf, s);
		refresh();
		switch(getch()) {
			case 'Q':
			case 'q':
				RUNNING = false;
				break;
			case 'T':
			case 't':
                ++conf->themeindex;
                conf->themeindex %= LENGTH(themes);
                conf->theme = themes[conf->themeindex];
				break;
			case 'C':
			case 'c':
                ++conf->colourindex;
                conf->colourindex %= LENGTH(sets);
                conf->colourset = sets[conf->colourindex];
				break;
			default:
				break;
		}
	}

	endwin();
	free(conf);
	free(s);
	return EXIT_SUCCESS;
}
