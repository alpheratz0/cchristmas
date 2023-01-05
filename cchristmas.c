/*
	This is free and unencumbered software released into the public domain.

	Anyone is free to copy, modify, publish, use, compile, sell, or
	distribute this software, either in source code form or as a compiled
	binary, for any purpose, commercial or non-commercial, and by any
	means.

	In jurisdictions that recognize copyright laws, the author or authors
	of this software dedicate any and all copyright interest in the
	software to the public domain. We make this dedication for the benefit
	of the public at large and to the detriment of our heirs and
	successors. We intend this dedication to be an overt act of
	relinquishment in perpetuity of all present and future rights to this
	software under copyright law.

*/

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>
#include <time.h>
#include <sys/ioctl.h>
#include <string.h>

static int  tree_height;
static int  tree_width;
static int *tree_light_colors;
static int  message_color;

static void
die(const char *fmt, ...)
{
	va_list args;

	fputs("cchristmas: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
	exit(1);
}

static int
get_new_year(void)
{
	time_t now;
	struct tm *tm;
	int new_year;

	now = time(NULL);
	tm = localtime(&now);
	new_year = (1900 + tm->tm_year) +
		!(tm->tm_mon == 0 && tm->tm_mday == 1);

	return new_year;
}

static void
term_get_size(int *width, int *height)
{
	struct winsize ws;

	*width = *height = 0;

	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
		*width = ws.ws_col;
		*height = ws.ws_row;
	}
}

static inline void
term_set_cursor_position(int x, int y)
{
	printf("\033[%d;%dH", y+1, x+1);
}

static inline void
term_set_foreground_color(int color)
{
	printf("\033[%dm", color);
}

static inline void
term_set_echo_flag(int set)
{
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag = set ? term.c_lflag | ECHO : term.c_lflag & ~ECHO;
	tcsetattr(STDIN_FILENO, 0, &term);
}

static inline void
term_hide_cursor(void)
{
	printf("\033[?25l");
}

static inline void
term_show_cursor(void)
{
	printf("\033[?25h");
}

static inline void
term_clear(void)
{
	printf("\033[H\033[J");
}

static void
set_signal_handler(int sig, void(*handler)(int))
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);

	sa.sa_flags = 0;
	sa.sa_handler = handler;

	if (sigaction(sig, &sa, NULL) == -1)
		die("couldn't register signal handler");
}

static void
h_sigint(int sig)
{
	(void) sig;
	term_show_cursor();
	term_set_echo_flag(1);
	term_clear();
	exit(0);
}

static void
h_winch(int sig)
{
	(void) sig;
	term_clear();
}

static void
tree_init(void)
{
	tree_height = 12;
	tree_width  = (tree_height - 2) * 2 - 1;
	tree_light_colors = calloc((tree_height - 2) * (tree_height - 2), sizeof(int));
}

static void
tree_render(int new_year)
{
	int level, light;
	int term_width, term_height;
	int trunk;
	int y;

	term_get_size(&term_width, &term_height);
	y = (term_height - tree_height - 2) / 2;

	for (level = 0; level < (tree_height - 2); ++level, ++y) {
		for (light = 0; light < (1 + level * 2); ++light) {
			term_set_cursor_position(term_width / 2 - level + light, y);
			term_set_foreground_color(tree_light_colors[level*level+light] + 32);
			printf(tree_light_colors[level*level+light] > 0 ? "o" : "*");
		}
	}

	term_set_foreground_color(33);

	for (trunk = 0; trunk < 2; ++trunk, ++y) {
		term_set_cursor_position(term_width / 2 - 1, y);
		printf("mWm");
	}

	term_set_foreground_color(message_color);

	y += 2;
	term_set_cursor_position((term_width - strlen("MERRY CHRISTMAS") + 2) / 2, y);
	printf("MERRY CHRISTMAS");

	y++;
	term_set_cursor_position((term_width - strlen("And lots of CODE in YYYY") + 1) / 2, y);
	printf("And lots of CODE in %d", new_year);
}

static void
tree_update(void)
{
	message_color = 31 + (rand()) % 7;

	tree_light_colors[rand() % ((tree_height - 2) * (tree_height - 2))] = 0;
	tree_light_colors[rand() % ((tree_height - 2) * (tree_height - 2))] = 0;
	tree_light_colors[rand() % ((tree_height - 2) * (tree_height - 2))] = 0;
	tree_light_colors[rand() % ((tree_height - 2) * (tree_height - 2))] = rand() % 6;
}

static void
usage(void)
{
	puts("usage: cchristmas [-hv]");
	exit(0);
}

static void
version(void)
{
	puts("cchristmas version "VERSION);
	exit(0);
}

int
main(int argc, char **argv)
{
	int new_year;

	while (++argv, --argc > 0) {
		if ((*argv)[0] == '-' && (*argv)[1] != '\0' && (*argv)[2] == '\0') {
			switch ((*argv)[1]) {
				case 'h': usage(); break;
				case 'v': version(); break;
				default: die("invalid option %s", *argv); break;
			}
		} else {
			die("unexpected argument: %s", *argv);
		}
	}

	new_year = get_new_year();

	tree_init();
	term_set_echo_flag(0);
	term_hide_cursor();
	term_clear();

	set_signal_handler(SIGINT, h_sigint);
	set_signal_handler(SIGWINCH, h_winch);

	for (;;) {
		tree_update();
		tree_render(new_year);
		fflush(stdout);
		usleep(1000 * 100);
	}

	return 0;
}
