/*
	Copyright (C) 2022 <alpheratz99@protonmail.com>

	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License version 2 as published by the
	Free Software Foundation.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "debug.h"

#define clear() printf("\033[H\033[J")
#define set_cursor(x,y) printf("\033[%d;%dH", (y), (x))
#define set_color(color) printf("\033[%dm", color);
#define show_cursor() printf("\033[?25h")
#define hide_cursor() printf("\033[?25l")

#define CENTER_X (size.columns / 2)
#define CENTER_Y (size.rows / 2)

#define START_LINE 2
#define CROWN_HEIGHT 10
#define TRUNK_HEIGHT 2

#define MERRY_CHRISTMAS "merry christmas"
#define CODE "code"
#define AND_LOTS_OF_CODE_IN "and lots of "CODE" in "
#define NEW_YEAR 2023

struct term_size {
	uint16_t rows;
	uint16_t columns;
};

struct term_size size;

static void 
update_size() {
    struct winsize ws;
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
		size.rows = ws.ws_row;
		size.columns = ws.ws_col;
	}
}

static void
on_resize(__attribute__((unused)) int sig) {
	clear();
	update_size();
}

static void
register_sigwinch() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = on_resize;
    if (sigaction(SIGWINCH, &sa, NULL) == -1)
		die("couldn't register handler for winch signal");
}

static void
render() {
	static int color = 0;
	color %= 7;
	set_cursor(CENTER_X, CENTER_Y);
	set_color(31 + color);
	printf("CODE");
	color++;
	if (color % 2 == 0) show_cursor();
	else hide_cursor();
}

int
main(void) {
	update_size();
	register_sigwinch();

	while(1) {
		render();
		fflush(stdout);
		usleep(1000 * 200);
	}

	return 0;
}
