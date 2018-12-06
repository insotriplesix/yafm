/********************************************************************
 * PROGRAM: yafm
 * FILE: init.c
 * PURPOSE: file manager initialization functions
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#include "gui.h"
#include "init.h"

/*
 * Function: initialize
 * --------------------
 * Description:
 *  Initializes the manager (run ncurses, enable raw mode, load
 *  configuration file etc).
 *
 * Arguments:
 *  'conf' - the flag that indicates do we need to take care about
 *           the config file or not.
 *
 * Notes:
 *  When we, for example, finished editing files in YAFM (via YATE) -
 *  we need to restore windows in their previous state. So, we don`t
 *  need to save or reload config files, because current parameters
 *  already in our memory.
 */

void
initialize(int conf)
{
	if (init_ncurses() || init_colors()) {
		endwin();
		fprintf(stderr, "Ncurses initializing error.\n");
		exit(EXIT_FAILURE);
	}

	if (LINES < 11 || COLS < 98) {
		endwin();
		fprintf(stderr, "Terminal window is too small.\n"
			"Min: 11x98, your: %dx%d\n", LINES, COLS);
		exit(EXIT_FAILURE);
	}

	init_windows();
	init_gui();
	init_content();

	if (conf) {
		get_config();
		get_editor_path();
		load_config();
	}

	if (display_content(LEFT_W) || display_content(RITE_W)) {
		endwin();
		fprintf(stderr, "Displaying error.\n");
		exit(EXIT_FAILURE);
	}

	enable_raw_mode();
}

/*
 * Function: finalize
 * ------------------
 * Description:
 *  Restores default terminal attributes, frees allocated memory
 *  and saves current manager configuration into the config file.
 *
 * Arguments:
 *  'conf' - the flag that indicates do we need to take care about
 *           the config file or not.
 */

void
finalize(int conf)
{
	disable_raw_mode();

	for (int i = 0; i < NWINDOWS; ++i) {
		list_mem_free(&content[i].files);
		delwin(win[i]);
	}

	endwin();

	if (conf) save_config();
}

/*
 * Function: init_colors
 * ---------------------
 * Description:
 *  Initializes color palette.
 *
 * Returns:
 *  'OK' (0 value) if initialization completed, 'ERR' otherwise.
 */

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

/*
 * Function: init_gui
 * ------------------
 * Description:
 *  Draws manager windows.
 */

void
init_gui(void)
{
	for (int i = 0; i < NWINDOWS; ++i) draw_window(i);
}

/*
 * Function: init_ncurses
 * ----------------------
 * Description:
 *  Ncurses library initialization.
 *
 * Returns:
 *  'OK' (0 value) only if all 4 functions return 0, 'ERR' otherwise.
 */

int
init_ncurses(void)
{
	return (initscr() != NULL) & clear() & cbreak() & noecho();
}

/*
 * Function: init_windows
 * ----------------------
 * Description:
 *  Allocates memory for editor windows and enables auxiliary stuff
 *  for user input such as control and function keys.
 *
 * Asserts:
 *  'newwin' won`t return NULL.
 */

void
init_windows(void)
{
	win[MENU_W] = newwin(3, COLS, 0, 0);
	assert(win[MENU_W] != NULL);

	win[LEFT_W] = newwin(LINES - 3, COLS / 2, 3, 0);
	assert(win[LEFT_W] != NULL);

	win[RITE_W] = newwin(LINES - 3, COLS / 2, 3, COLS / 2);
	assert(win[RITE_W] != NULL);

	curs_set(0);

	// Enable scrolling, func keys, arrows etc.
	keypad(win[LEFT_W], TRUE);
	keypad(win[RITE_W], TRUE);

	ACTIVE_W = LEFT_W;
}
