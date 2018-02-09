#include "gui.h"
#include "init.h"

void
finalize(void)
{
	for (int i = 0; i < NWINDOWS; ++i) {
		list_mem_free(&content[i].files);
		delwin(win[i]);
	}

	endwin();

}

void
initialize(void)
{
	if (init_ncurses() | init_colors() | init_windows() | init_gui()) {
		endwin();
		fprintf(stderr, "Initializing error.\n");
		exit(EXIT_FAILURE);
	}

	if (LINES < 6 || COLS < 98) {
		endwin();
		fprintf(stderr, "Terminal window is too small.\n"
			"Min: 6x98, your: %dx%d\n", LINES, COLS);
		exit(EXIT_FAILURE);
	}

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		endwin();
		fprintf(stderr, "Displaying error.\n");
		exit(EXIT_FAILURE);
	}

	mvwchgat(win[ACTIVE_W], DEFPOS_Y, DEFPOS_X, COLS / 2 - 2,
		A_NORMAL, CURSOR_C, NULL);
}

int
init_colors(void)
{
	if (start_color() == ERR || !has_colors()) {
		fprintf(stderr, "This terminal doesn't support colouring.\n");
		return ERR;
	}

	// Pair id, Foreground, Background
	init_pair(1, COLOR_BLACK, COLOR_YELLOW);	// menu components
	init_pair(2, COLOR_WHITE, COLOR_BLUE);		// edit field
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);	// pop-up windows
	init_pair(4, COLOR_WHITE, COLOR_BLACK);		// borders

	init_pair(5, COLOR_YELLOW, COLOR_BLUE);		// directories
	init_pair(6, COLOR_WHITE, COLOR_BLUE);		// regular files
	init_pair(7, COLOR_GREEN, COLOR_BLUE);		// executables
	init_pair(8, COLOR_MAGENTA, COLOR_BLUE);	// other

	init_pair(9, COLOR_WHITE, COLOR_RED);		// cursor highlight

	return OK;
}

int
init_gui(void)
{
	for (int i = 0; i < NWINDOWS; ++i)
		draw_window(i);

	return OK;
}

int
init_ncurses(void)
{
	return (initscr() != NULL) & clear() & cbreak() & noecho();
}

int
init_windows(void)
{
	win[MENU_W] = newwin(3, COLS, 0, 0);
	if (win[MENU_W] == NULL) {
		perror("newwin");
		return ERR;
	}

	win[LEFT_W] = newwin(LINES - 3, COLS / 2, 3, 0);
	if (win[LEFT_W] == NULL) {
		perror("newwin");
		return ERR;
	}

	win[RITE_W] = newwin(LINES - 3, COLS / 2, 3, COLS / 2);
	if (win[RITE_W] == NULL) {
		perror("newwin");
		return ERR;
	}

	// Enable scrolling, func keys, arrows etc.
	curs_set(0);
	keypad(win[LEFT_W], TRUE);
	keypad(win[RITE_W], TRUE);

	// Prepare content stuff
	for (int i = 0; i < NWINDOWS; ++i) {
		content[i].x_pos = 1;
		content[i].y_pos = 1;
		content[i].count = 0;
		content[i].y_off = 0;
		getcwd(content[i].path, PATH_MAX);
		list_init(&content[i].files);
	}

	ACTIVE_W = LEFT_W;

	return OK;
}
