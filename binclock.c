/*
 *   Copyright 2011, 2012 John Anthony and licensed unde rthe GPLv3
 *   See README.md and LICENSE files for more information
 */

#include <assert.h>
#include <getopt.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LENGTH(x) (( sizeof(x) / sizeof(x[0]) ))

/* Typedefs */

typedef struct Theme {
	char left;
	char right;
	char off;
	char on;
} Theme;

typedef struct Config {
	Theme theme;
} Config;

/* Function Predecs */

static void draw_time(Config *conf);
static Config* handle_args(int argc, char **argv);
static bool init(void);
static void usage_and_exit(int status);

/* Functions */

static Config*
handle_args(int argc, char **argv) {
	static Theme themes[] = {
		{'[', ']', ' ', '*'},
		{'<', '>', '0', '1'},
		{'(', ')', ' ', '+'},
	};
	Config *conf;
	int i;
	int tmp;

	conf = malloc(sizeof(Config));
	assert(conf);

	/* Initialise defaults */
	conf->theme = themes[0];

	/* Actual parsing */
	while( (i = getopt(argc, argv, "ht:")) != -1 ) {
		switch(i) {
		case 'h':
			free(conf);
			usage_and_exit(EXIT_SUCCESS);
			break;
		case 't':
			tmp = atoi(optarg);
			assert(tmp >= 0 && tmp <= LENGTH(themes));
			conf->theme = themes[tmp];
			break;
		case '?':
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

	conf = handle_args(argc, argv);
	assert( init() );
	while (RUNNING) {
		refresh();
		drawtime(conf);
		refresh();
		sleep(1);
	}

	endwin();
	free(conf);
	return EXIT_SUCCESS;
}
