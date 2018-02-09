#include "readkey.h"

int
readkey(void)
{
	int ch, rc = OK, status;
	char *name;

	ch = wgetch(win[ACTIVE_W]);

	switch (ch) {
		// Movements
		case KEY_LEFT:
			move_left();
			break;
		case KEY_RIGHT:
			move_right();
			break;
		case KEY_UP:
			move_up();
			break;
		case KEY_DOWN:
			move_down();
			break;
		case KEY_HOME:
			set_default_attr();
			content[ACTIVE_W].y_pos = DEFPOS_Y;
			wmove(win[ACTIVE_W], content[ACTIVE_W].y_pos,
				content[ACTIVE_W].x_pos);
			break;
		case KEY_END:
			set_default_attr();
			content[ACTIVE_W].y_pos = content[ACTIVE_W].count;
			wmove(win[ACTIVE_W], content[ACTIVE_W].y_pos,
				content[ACTIVE_W].x_pos);
			break;
		// File options
		case KEY_F(4):
		case CTRL_O:
			rc = get_file_opt();
			break;
		// Dir options
		case KEY_F(5):
		case CTRL_K:
			rc = get_dir_opt();
			break;
		// Extra options
		case KEY_F(6):
		case CTRL_E:
			rc = get_extra_opt();
			break;
		// Help window
		case KEY_F(7):
		case CTRL_F:
			rc = get_help();
			break;
		// Exit
		case KEY_F(8):
		case CTRL_X:
			finalize();
			exit(EXIT_SUCCESS);
			break;
		// Change theme
		case CTRL_G:
			rc = change_theme();
			break;
		// Edit / exec / chdir
		case KEY_ENTER:
		case KEY_NL:
			name = list_find_data(&content[ACTIVE_W].files,
				content[ACTIVE_W].y_pos);

			if (name == NULL) // output err
				break;

			curs_set(1);

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
					if (fork() == 0) {
						rc = edit_file(name);
					} else {
						wait(&status);
						finalize();
						restore_windows();
					}
					break;
			}
			curs_set(0);
			break;
		default:
			break;
	}

	return rc;
}
