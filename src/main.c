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
			"Min: 11x98, your: %dx%d\n", LINES, COLS);
		exit(EXIT_FAILURE);
	}

	int defpos_y = 1, defpos_x = 1;
	int curpos_y = defpos_y, curpos_x = defpos_x;

	int is_exit = FALSE;

	if (display_content(LEFT_W) | display_content(RITE_W)) {
		endwin();
		fprintf(stderr, "Cannot display directory files.\n");
		exit(EXIT_FAILURE);
	}

	while (!is_exit) {
		wmove(win[ACTIVE_W], curpos_y, curpos_x);
		ch = wgetch(win[1]);

		switch (ch) {
			// Movements
			case KEY_LEFT:
				move_left();
				break;
			case KEY_RIGHT:
				move_right();
				break;
			case KEY_UP:
				move_up(0);
				break;
			case KEY_DOWN:
				move_down(LINES - 4);
				break;
			case KEY_HOME:
				curpos_x = defpos_x;
				break;
			case KEY_END:
				curpos_x = COLS - 2;
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
/*			case CTRL_W:
				rc = hex_editor(win, height, width);

				if (rc == ERR) {
					fprintf(stderr, "Error in hex_editor() func.\n");
					is_exit = TRUE;
				}
				break;
*/			// Change theme
			case CTRL_G:
				rc = change_theme();
				if (rc == ERR) is_exit = TRUE;
				break;
			// Print character
			default:
				break;
/*				if (iscntrl(ch))
					break;

				if (encryption == TRUE)
					ch ^= 1;

				filebuf_pos = curpos_y * COLS + curpos_x;
				if (filebuf_size < BUFSIZ || filebuf_pos < filebuf_size)
					filebuf[filebuf_size++] = (char) ch;

				if (curpos_x + 1 < COLS - 1) {
					waddch(win[1], ch);
					move_right(&curpos_y, &curpos_x, COLS);
				} else {
					// Deal with eol
					waddch(win[1], ch);
					curpos_x = defpos_x;
					++curpos_y;
				}
*/		}

		// Update options bar
		touchwin(win[0]);
		wnoutrefresh(win[0]);

		// Update info bar
//		mvwprintw(win[2], 0, COLS / 2 - 6, " %3d : %3d ", curpos_y, curpos_x);
//		touchwin(win[2]);
//		wnoutrefresh(win[2]);

		curpos_y = content[ACTIVE_W].y_pos;
		curpos_x = content[ACTIVE_W].x_pos;

		// Update text field
		wmove(win[LEFT_W], curpos_y, curpos_x);
		touchwin(win[LEFT_W]);
		wnoutrefresh(win[LEFT_W]);

		wmove(win[RITE_W], curpos_y, curpos_x);
		touchwin(win[RITE_W]);
		wnoutrefresh(win[RITE_W]);

		doupdate();
	}

	for (int i = 0; i < NWINDOWS; ++i)
		delwin(win[i]);

	endwin();

	return rc;
}
