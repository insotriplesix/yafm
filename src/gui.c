/********************************************************************
 * PROGRAM: yafm
 * FILE: gui.c
 * PURPOSE: manager gui
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#include "gui.h"

/*
 * Function: update_gui
 * --------------------
 * Description:
 *  Updates program gui.
 */

void
update_gui(void)
{
	for (int i = 0; i < NWINDOWS; ++i) {
		touchwin(win[i]);
		wnoutrefresh(win[i]);
	}

	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, LIMIT_X, A_NORMAL,
		CURSOR_C, NULL);

	doupdate();
}

/*
 * Function: draw_window
 * ---------------------
 * Description:
 *  Draws a window.
 *
 * Arguments:
 *  'wtype' - specifies the window to draw (e.g. menu).
 */

void
draw_window(enum win_t wtype)
{
	wclear(win[wtype]);

	wattron(win[wtype], BORDER_CLR);
	box(win[wtype], ACS_VLINE, ACS_HLINE);
	wattroff(win[wtype], BORDER_CLR);

	switch (wtype) {
		case MENU_W: draw_menu(wtype); break;
		case LEFT_W: draw_left(wtype); break;
		case RITE_W: draw_rite(wtype); break;
		default: break;
	}

	wrefresh(win[wtype]);
}

/*
 * Function: draw_left
 * -------------------
 * Description:
 *  Draws the left part of the manager.
 *
 * Arguments:
 *  'wtype' - type of a window (LEFT_W in this case).
 */

void
draw_left(enum win_t wtype)
{
	wbkgd(win[wtype], EDIT_CLR);
}

/*
 * Function: draw_rite
 * -------------------
 * Description:
 *  Draws the right part of the manager.
 *
 * Arguments:
 *  'wtype' - type of a window (RITE_W in this case).
 */

void
draw_rite(enum win_t wtype)
{
	wbkgd(win[wtype], EDIT_CLR);
}

/*
 * Function: draw_menu
 * -------------------
 * Description:
 *  Draws the menu window.
 *
 * Arguments:
 *  'wtype' - type of a window (MENU_W in this case).
 */

void
draw_menu(enum win_t wtype)
{
	wbkgd(win[wtype], BORDER_CLR);

	int offset = 2;

	wattron(win[wtype], MENU_CLR);
	mvwprintw(win[wtype], 1, offset, "  F4 - File  ");
	offset += 15;
	mvwprintw(win[wtype], 1, offset, "  F5 - Dir  ");
	offset += 14;
	mvwprintw(win[wtype], 1, offset, "  F6 - Extra  ");
	offset += 16;
	mvwprintw(win[wtype], 1, offset, "  F7 - Help  ");
	offset += 15;
	mvwprintw(win[wtype], 1, offset, "  F8 - Exit  ");
	mvwprintw(win[wtype], 1, COLS - 20, " made by 5aboteur ");
	wattroff(win[wtype], MENU_CLR);
}

/*
 * Function: change_theme
 * ----------------------
 * Description:
 *  Changes current theme to another one that was received from the
 *  config file or selected by user.
 *
 * Arguments:
 *  'popup' - the flag that indicates wheter a popup window will be
 *            displayed or not.
 */

void
change_theme(int popup)
{
	int fg_field, bg_field, fg_menu, bg_menu,
		fg_popup, bg_popup, fg_dir, bg_dir,
		fg_reg, bg_reg, fg_exec, bg_exec,
		fg_oth, bg_oth, fg_curs, bg_curs;

	if (popup) current_theme = (char)change_theme_popup();

	switch (current_theme) {
		case '0': // default
			fg_menu = COLOR_BLACK, bg_menu = COLOR_YELLOW;
			fg_field = COLOR_WHITE, bg_field = COLOR_BLUE;
			fg_popup = COLOR_YELLOW, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_YELLOW, bg_dir = COLOR_BLUE;
			fg_reg = COLOR_WHITE, bg_reg = COLOR_BLUE;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_BLUE;
			fg_oth = COLOR_MAGENTA, bg_oth = COLOR_BLUE;
			fg_curs = COLOR_WHITE, bg_curs = COLOR_RED;
			break;
		case '1': // leet
			fg_menu = COLOR_BLACK, bg_menu = COLOR_GREEN;
			fg_field = COLOR_GREEN, bg_field = COLOR_BLACK;
			fg_popup = COLOR_GREEN, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_YELLOW, bg_dir = COLOR_BLACK;
			fg_reg = COLOR_WHITE, bg_reg = COLOR_BLACK;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_BLACK;
			fg_oth = COLOR_MAGENTA, bg_oth = COLOR_BLACK;
			fg_curs = COLOR_BLACK, bg_curs = COLOR_GREEN;
			break;
		case '2': // icy
			fg_menu = COLOR_CYAN, bg_menu = COLOR_BLACK;
			fg_field = COLOR_MAGENTA, bg_field = COLOR_WHITE;
			fg_popup = COLOR_CYAN, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_BLUE, bg_dir = COLOR_WHITE;
			fg_reg = COLOR_BLACK, bg_reg = COLOR_WHITE;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_WHITE;
			fg_oth = COLOR_RED, bg_oth = COLOR_WHITE;
			fg_curs = COLOR_WHITE, bg_curs = COLOR_MAGENTA;
			break;
		case '3': // hell
			fg_menu = COLOR_BLACK, bg_menu = COLOR_RED;
			fg_field = COLOR_RED, bg_field = COLOR_BLACK;
			fg_popup = COLOR_RED, bg_popup = COLOR_BLACK;
			fg_dir = COLOR_YELLOW, bg_dir = COLOR_BLACK;
			fg_reg = COLOR_WHITE, bg_reg = COLOR_BLACK;
			fg_exec = COLOR_GREEN, bg_exec = COLOR_BLACK;
			fg_oth = COLOR_MAGENTA, bg_oth = COLOR_BLACK;
			fg_curs = COLOR_WHITE, bg_curs = COLOR_RED;
			break;
		default:
			//status
			return;
	}

	init_pair(1, fg_menu, bg_menu);
	wattrset(win[MENU_W], MENU_CLR);

	init_pair(2, fg_field, bg_field);
	wattrset(win[LEFT_W], EDIT_CLR);

	init_pair(3, fg_popup, bg_popup);
	wattrset(win[RITE_W], POPUP_CLR);

	init_pair(5, fg_dir, bg_dir);
	wattrset(win[LEFT_W], DIR_CLR);
	wattrset(win[RITE_W], DIR_CLR);

	init_pair(6, fg_reg, bg_reg);
	wattrset(win[LEFT_W], REG_CLR);
	wattrset(win[RITE_W], REG_CLR);

	init_pair(7, fg_exec, bg_exec);
	wattrset(win[LEFT_W], EXE_CLR);
	wattrset(win[RITE_W], EXE_CLR);

	init_pair(8, fg_oth, bg_oth);
	wattrset(win[LEFT_W], SYM_CLR);
	wattrset(win[RITE_W], SYM_CLR);

	init_pair(9, fg_curs, bg_curs);
	wattrset(win[LEFT_W], CURSOR_CLR);
	wattrset(win[RITE_W], CURSOR_CLR);

	if (popup) {
		//status
	}
}

/*
 * Function: change_theme_popup
 * ----------------------------
 * Description:
 *  Draws 'themes' popup window.
 *
 * Asserts:
 *  'newpad' won`t return NULL.
 *
 * Returns:
 *  'choice' value that contains a theme number.
 */

int
change_theme_popup(void)
{
	WINDOW *win;

	int win_height = 6;
	int win_width = 13;
	int line = 1;
	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newpad(win_height, win_width);

	assert(win != NULL);

	wattron(win, BORDER_CLR);
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, BORDER_CLR);
	wbkgd(win, POPUP_CLR);

	wmove(win, line++, 1);
	waddstr(win, " [0] def  ");
	wmove(win, line++, 1);
	waddstr(win, " [1] leet ");
	wmove(win, line++, 1);
	waddstr(win, " [2] icy  ");
	wmove(win, line++, 1);
	waddstr(win, " [3] hell ");

	prefresh(win, 0, 0,
		offset_y - win_height / 2,
		offset_x - win_width / 2,
		offset_y + win_height / 2,
		offset_x + win_width / 2);

	int choice = wgetch(win);

	flushinp();
	wclear(win);
	wrefresh(win);
	delwin(win);

	return choice;
}

/*
 * Function: get_file_opt
 * ----------------------
 * Description:
 *  Draws 'file' popup window.
 *
 * Asserts:
 *  'newpad' won`t return NULL.
 */

void
get_file_opt(void)
{
	WINDOW *win;

	int win_height = 5;
	int win_width = 13;
	int line = 0;
	int offset_y = 2;
	int offset_x = 2;

	win = newpad(win_height, win_width);

	assert(win != NULL);

	wbkgd(win, MENU_CLR);

	wmove(win, line++, 1);
	waddstr(win, "-----------");
	wmove(win, line++, 1);
	waddstr(win, "^Q - create");
	wmove(win, line++, 1);
	waddstr(win, "^C - copy");
	wmove(win, line++, 1);
	waddstr(win, "^V - paste");
	wmove(win, line++, 1);
	waddstr(win, "^R - remove");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	flushinp();
	wclear(win);
	wrefresh(win);
	delwin(win);
}

/*
 * Function: get_dir_opt
 * ---------------------
 * Description:
 *  Draws 'dir' popup window.
 *
 * Asserts:
 *  'newpad' won`t return NULL.
 */

void
get_dir_opt(void)
{
	WINDOW *win;

	int win_height = 3;
	int win_width = 12;
	int line = 0;
	int offset_y = 2;
	int offset_x = 17;

	win = newpad(win_height, win_width);

	assert(win != NULL);

	wbkgd(win, MENU_CLR);

	wmove(win, line++, 1);
	waddstr(win, "----------");
	wmove(win, line++, 1);
	waddstr(win, "^W - mkdir");
	wmove(win, line++, 1);
	waddstr(win, "^K - rmdir");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	flushinp();
	wclear(win);
	wrefresh(win);
	delwin(win);
}

/*
 * Function: get_extra_opt
 * -----------------------
 * Description:
 *  Draws 'extra' popup window.
 *
 * Asserts:
 *  'newpad' won`t return NULL.
 */

void
get_extra_opt(void)
{
	WINDOW *win;

	int win_height = 2;
	int win_width = 14;
	int line = 0;
	int offset_y = 2;
	int offset_x = 31;

	win = newpad(win_height, win_width);

	assert(win != NULL);

	wbkgd(win, MENU_CLR);

	wmove(win, line++, 1);
	waddstr(win, "------------");
	wmove(win, line++, 1);
	waddstr(win, "^G - themez");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	flushinp();
	wclear(win);
	wrefresh(win);
	delwin(win);
}

/*
 * Function: get_help
 * ------------------
 * Description:
 *  Draws 'help' popup window.
 *
 * Asserts:
 *  'newpad' won`t return NULL.
 */

void
get_help(void)
{
	WINDOW *win;

	int win_height = (LINES >= 10) ? 10 : 7;
	int win_width = (LINES >= 10) ? 26 : 42;
	int line = 1;
	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newpad(win_height, win_width);

	assert(win != NULL);

	wattron(win, BORDER_CLR);
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, BORDER_CLR);
	wbkgd(win, POPUP_CLR);

	if (LINES >= 10) {
		wmove(win, line++, win_width / 3);
		waddstr(win, "YAFM v1.1f");
		wmove(win, line++, 1);
		waddstr(win, "");
		wmove(win, line++, 1);
		waddstr(win, "--- cmd ----------------");
		wmove(win, line++, 1);
		waddstr(win, " F4, ^F - file options");
		wmove(win, line++, 1);
		waddstr(win, " F5, ^D - dir options");
		wmove(win, line++, 1);
		waddstr(win, " F6, ^E - extra options");
		wmove(win, line++, 1);
		waddstr(win, " F7, ^I - get help");
		wmove(win, line++, 1);
		waddstr(win, " F8, ^X - exit");
	} else {
		wmove(win, line++, 1);
		waddstr(win, " F4, ^F - file | ^Q - creat, ^C - copy");
		wmove(win, line++, 1);
		waddstr(win, "               | ^V - paste, ^R - remov");
		wmove(win, line++, 1);
		waddstr(win, " F5, ^D - dir  | ^W - mkdir, ^K - rmdir");
		wmove(win, line++, 1);
		waddstr(win, " F6, ^E - ext  | ^G - themez");
		wmove(win, line++, 1);
		waddstr(win, " F7, ^I - help | F8, ^X - exit");
	}

	prefresh(win, 0, 0,
		offset_y - win_height / 2,
		offset_x - win_width / 2,
		offset_y + win_height / 2,
		offset_x + win_width / 2);

	wgetch(win);

	flushinp();
	wclear(win);
	wrefresh(win);
	delwin(win);
}

/*
 * Function: create_file_popup
 * ---------------------------
 * Description:
 *  Draws popup window which is waits for the input.
 *
 * Arguments:
 *  'fname' - an array that stores the name of a file
 *            to be created.
 *
 * Asserts:
 *  'newwin' won`t return NULL.
 */

void
create_file_popup(char *fname)
{
	WINDOW *win;

	int win_height = 3;
	int win_width = 42;
	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newwin(win_height, win_width,
		offset_y - win_height / 2,
		offset_x - win_width / 2);

	assert(win != NULL);

	wattron(win, BORDER_CLR);
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, BORDER_CLR);
	wbkgd(win, POPUP_CLR);

	echo();

	mvwaddstr(win, 1, 1, " Enter file name: ");
	wrefresh(win);

	mvwgetstr(win, 1, 19, fname);

	flushinp();
	noecho();

	wclear(win);
	wrefresh(win);
	delwin(win);
}

/*
 * Function: make_dir_popup
 * ------------------------
 * Description:
 *  Draws popup window which is waits for the input.
 *
 * Arguments:
 *  'dname' - an array that stores the name of a dir
 *            to be created.
 *
 * Asserts:
 *  'newwin' won`t return NULL.
 */

void
make_dir_popup(char *dname)
{
	WINDOW *win;

	int win_height = 3;
	int win_width = 42;

	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newwin(win_height, win_width,
		offset_y - win_height / 2,
		offset_x - win_width / 2);

	assert(win != NULL);

	wattron(win, BORDER_CLR);
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, BORDER_CLR);
	wbkgd(win, POPUP_CLR);

	echo();

	mvwaddstr(win, 1, 1, " Enter dir name: ");
	wrefresh(win);

	mvwgetstr(win, 1, 18, dname);

	flushinp();
	noecho();

	wclear(win);
	wrefresh(win);
	delwin(win);
}
