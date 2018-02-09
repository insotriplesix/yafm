#include "fmanager.h"
#include "init.h"
#include "movement.h"

int
main(/*int argc, char *argv[]*/)
{
	int ch, rc = OK, status;
	char *name;
	int is_exit = FALSE;

	initialize();

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
				content[ACTIVE_W].y_pos = DEFPOS_Y;
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
			// Edit / exec / chdir
			case KEY_ENTER:
			case KEY_NL:
				name = list_find_data(&content[ACTIVE_W].files, content[ACTIVE_W].y_pos);

				if (name == NULL) // output err
					break;

				switch (name[0]) {
					case '/':
						set_default_attr();
						rc = change_dir_to(name);
						break;
					case '*':
						if (fork() == 0) {
							rc = exec_prog(name);
						} else {
							wait(&status);
							finalize();
							restore_windows();
						}
						break;
					case '~':
						break;
					default:
						break;
				}

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
			COLS / 2 - 2, A_NORMAL, CURSOR_CLR, NULL);

		doupdate();
	}

	finalize();

	return rc;
}
