#include "init.h"

char filename[FILENAME_MAX];

/* Init ncurses library */
int
init_ncurses()
{
	return (initscr() != NULL) & clear() & cbreak() & noecho();
}

/* Init color palette */
int
init_colors()
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

	init_pair(9, COLOR_WHITE, COLOR_RED);	// cursor highlight

	return OK;
}

/* Init default windows */
int
init_windows()
{
	win[MENU_W] = newwin(3, COLS, 0, 0);

	if (win[MENU_W] == NULL) {
		perror("newwin");
		return ERR;
	}

	wattron(win[MENU_W], COLOR_PAIR(4));
	box(win[MENU_W], ACS_VLINE, ACS_HLINE);
	wattroff(win[MENU_W], COLOR_PAIR(4));
	wbkgd(win[MENU_W], COLOR_PAIR(4));

	int offset = 2;

	wattron(win[MENU_W], COLOR_PAIR(1));
	mvwprintw(win[MENU_W], 1, offset, "  F4 - File  ");
	offset += 15;
	mvwprintw(win[MENU_W], 1, offset, "  F5 - ????  ");
	offset += 15;
	mvwprintw(win[MENU_W], 1, offset, "  F6 - Extra  ");
	offset += 16;
	mvwprintw(win[MENU_W], 1, offset, "  F7 - Help  ");
	offset += 15;
	mvwprintw(win[MENU_W], 1, offset, "  F8 - Exit  ");
	mvwprintw(win[MENU_W], 1, COLS - 20, " made by 5aboteur ");
	wattroff(win[MENU_W], COLOR_PAIR(1));

	wrefresh(win[MENU_W]);

	win[LEFT_W] = newwin(LINES - 3, COLS / 2, 3, 0);

	if (win[LEFT_W] == NULL) {
		perror("newwin");
		return ERR;
	}

	wattron(win[LEFT_W], COLOR_PAIR(4));
	box(win[LEFT_W], ACS_VLINE, ACS_HLINE);
	wattroff(win[LEFT_W], COLOR_PAIR(4));
	wbkgd(win[LEFT_W], COLOR_PAIR(2));

	wrefresh(win[LEFT_W]);

	win[RITE_W] = newwin(LINES - 3, COLS / 2, 3, COLS / 2);

	if (win[RITE_W] == NULL) {
		perror("newwin");
		return ERR;
	}

	wattron(win[RITE_W], COLOR_PAIR(4));
	box(win[RITE_W], ACS_VLINE, ACS_HLINE);
	wattroff(win[RITE_W], COLOR_PAIR(4));
	wbkgd(win[RITE_W], COLOR_PAIR(2));

	wrefresh(win[RITE_W]);

	// Enable scrolling, func keys, arrows etc.
	keypad(win[LEFT_W], TRUE);
	wmove(win[LEFT_W], 1, 1);
	scrollok(win[LEFT_W], TRUE);
	idlok(win[LEFT_W], TRUE);
	wrefresh(win[LEFT_W]);

	keypad(win[RITE_W], TRUE);
	wmove(win[RITE_W], 1, 1);
	scrollok(win[RITE_W], TRUE);
	idlok(win[RITE_W], TRUE);
	wrefresh(win[RITE_W]);

	// Prepare content stuff
	for (int i = 0; i < NWINDOWS; ++i) {
		content[i].x_pos = 1;
		content[i].y_pos = 1;
		content[i].count = 0;
		getcwd(content[i].path, PATH_MAX);
		list_init(&content[i].files);
	}

	curs_set(0);
	ACTIVE_W = LEFT_W;

	return OK;
}
