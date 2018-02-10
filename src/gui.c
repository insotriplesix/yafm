#include "gui.h"

void
draw_left(enum win_t wtype)
{
	wbkgd(win[wtype], EDIT_CLR);
}

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

void
draw_rite(enum win_t wtype)
{
	wbkgd(win[wtype], EDIT_CLR);
}

void
draw_window(enum win_t wtype)
{
	wclear(win[wtype]);

	wattron(win[wtype], BORDER_CLR);
	box(win[wtype], ACS_VLINE, ACS_HLINE);
	wattroff(win[wtype], BORDER_CLR);

	switch (wtype) {
		case MENU_W:
			draw_menu(wtype);
			break;
		case LEFT_W:
			draw_left(wtype);
			break;
		case RITE_W:
			draw_rite(wtype);
			break;
		default:
			break;
	}

	wrefresh(win[wtype]);
}

int
change_theme(void)
{
	int fg_field, bg_field, fg_menu, bg_menu,
		fg_popup, bg_popup, fg_dir, bg_dir,
		fg_reg, bg_reg, fg_exec, bg_exec,
		fg_oth, bg_oth, fg_curs, bg_curs;

	int choice = change_theme_popup();
	if (choice == ERR) return ERR;

	switch ((char)choice) {
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
			return OK;
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

	return OK;
}

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

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

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

	wclear(win);
	wrefresh(win);
	delwin(win);

	return choice;
}

int
get_file_opt(void)
{
	WINDOW *win;

	int win_height = 6;
	int win_width = 13;
	int line = 0;

	int offset_y = 2;
	int offset_x = 2;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wbkgd(win, MENU_CLR);

	wmove(win, line++, 1);
	waddstr(win, "-----------");
	wmove(win, line++, 1);
	waddstr(win, "^A - create");
	wmove(win, line++, 1);
	waddstr(win, "^B - edit");
	wmove(win, line++, 1);
	waddstr(win, "^C - copy");
	wmove(win, line++, 1);
	waddstr(win, "^D - move");
	wmove(win, line++, 1);
	waddstr(win, "^D - remove");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

	return OK;
}

int
get_dir_opt(void)
{
	WINDOW *win;

	int win_height = 5;
	int win_width = 12;
	int line = 0;

	int offset_y = 2;
	int offset_x = 17;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wbkgd(win, MENU_CLR);

	wmove(win, line++, 1);
	waddstr(win, "----------");
	wmove(win, line++, 1);
	waddstr(win, "^A - mkdir");
	wmove(win, line++, 1);
	waddstr(win, "^B - copy");
	wmove(win, line++, 1);
	waddstr(win, "^C - move");
	wmove(win, line++, 1);
	waddstr(win, "^D - rmdir");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

	return OK;
}

int
get_extra_opt(void)
{
	WINDOW *win;

	int win_height = 3;
	int win_width = 14;
	int line = 0;

	int offset_y = 2;
	int offset_x = 31;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wbkgd(win, MENU_CLR);

	wmove(win, line++, 1);
	waddstr(win, "------------");
	wmove(win, line++, 1);
	waddstr(win, "^A - resize");
	wmove(win, line++, 1);
	waddstr(win, "^B - themez");

	prefresh(win, 0, 0,
		offset_y, offset_x,
		offset_y + win_height,
		offset_x + win_width);

	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

	return OK;
}

int
get_help(void)
{
	WINDOW *win;

	int win_height = 10;
	int win_width = 26;
	int line = 1;

	int offset_y = LINES / 2;
	int offset_x = COLS / 2;

	win = newpad(win_height, win_width);

	if (win == NULL) {
		perror("newpad");
		return ERR;
	}

	wattron(win, BORDER_CLR);
	box(win, ACS_VLINE, ACS_HLINE);
	wattroff(win, BORDER_CLR);
	wbkgd(win, POPUP_CLR);

	wmove(win, line++, win_width / 3);
	waddstr(win, "YAFM v1.0");
	wmove(win, line++, 1);
	waddstr(win, "");
	wmove(win, line++, 1);
	waddstr(win, "--- cmd ----------------");
	wmove(win, line++, 1);
	waddstr(win, " F4, ^O - file options");
	wmove(win, line++, 1);
	waddstr(win, " F5, ^K - dir options");
	wmove(win, line++, 1);
	waddstr(win, " F6, ^E - extra options");
	wmove(win, line++, 1);
	waddstr(win, " F7, ^F - get help");
	wmove(win, line++, 1);
	waddstr(win, " F8, ^X - exit");

	prefresh(win, 0, 0,
		offset_y - win_height / 2,
		offset_x - win_width / 2,
		offset_y + win_height / 2,
		offset_x + win_width / 2);

	wgetch(win);

	wclear(win);
	wrefresh(win);
	delwin(win);

	return OK;
}
