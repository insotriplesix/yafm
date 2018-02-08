#include "fmanager.h"
#include "init.h"
#include "movement.h"

int
main(/*int argc, char *argv[]*/)
{
	int ch, rc = OK;

	if (init_ncurses() | init_colors() | init_windows()) {
		endwin();
		fprintf(stderr, "Initialize stage error.\n");
		exit(EXIT_FAILURE);
	}

	if (LINES < 6 || COLS < 98) {
		endwin();
		fprintf(stderr, "Terminal window is too small.\n"
			"Min: 6x98, your: %dx%d\n", LINES, COLS);
		exit(EXIT_FAILURE);
	}

	int defpos_y = 1, defpos_x = 1;
//	int *curpos_y, *curpos_x;

	int is_exit = FALSE;

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		endwin();
		fprintf(stderr, "Cannot display directory files.\n");
		exit(EXIT_FAILURE);
	}

	mvwchgat(win[ACTIVE_W], defpos_y, defpos_x, COLS / 2 - 2, A_NORMAL, 9, NULL);

	while (!is_exit) {
		ch = wgetch(win[ACTIVE_W]);

		switch (ch) {
			// Movements
			case KEY_LEFT:
				if (ACTIVE_W == RITE_W) {
					dim_cursor();
					ACTIVE_W = LEFT_W;
				}
				break;
			case KEY_RIGHT:
				if (ACTIVE_W == LEFT_W) {
					dim_cursor();
					ACTIVE_W = RITE_W;
				}
				break;
			case KEY_UP:
				if (content[ACTIVE_W].y_pos - 1 > 0) {
					set_default_attr();
					content[ACTIVE_W].y_pos--;
				}
				break;
			case KEY_DOWN:
				if (content[ACTIVE_W].y_pos + 1 < content[ACTIVE_W].count + 1) {
					set_default_attr();
					content[ACTIVE_W].y_pos++;
				}
				break;
			case KEY_HOME:
				set_default_attr();
				content[ACTIVE_W].y_pos = defpos_y;
				wmove(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos);
				break;
			case KEY_END:
				set_default_attr();
				content[ACTIVE_W].y_pos = content[ACTIVE_W].count;
				wmove(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos);
				break;
			// Open file
			case KEY_F(4):
			case CTRL_O:
				break;
			// Save to file
			case KEY_F(5):
			case CTRL_K:
				break;
			// Extra
			case KEY_F(6):
			case CTRL_E:
				rc = get_extra();
				if (rc == ERR) is_exit = TRUE;
				break;
			// Help
			case KEY_F(7):
			case CTRL_F:
				rc = get_help();
				if (rc == ERR) is_exit = TRUE;
				break;
			// Exit
			case KEY_F(8):
			case CTRL_X:
				is_exit = TRUE;
				break;
			// Change theme
			case CTRL_G:
				rc = change_theme();
				if (rc == ERR) is_exit = TRUE;
				break;
			// Print character
			default:
				break;
		}

		// Update manager windows
		for (int i = 0; i < NWINDOWS; ++i) {
			touchwin(win[i]);
			wnoutrefresh(win[i]);
		}

		mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos,
			COLS / 2 - 2, A_NORMAL, 9, NULL);

		doupdate();
	}

	for (int i = 0; i < NWINDOWS; ++i)
		delwin(win[i]);

	endwin();

	return rc;
}
