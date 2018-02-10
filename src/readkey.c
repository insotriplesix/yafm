#include "readkey.h"

int
readkey(void)
{
	int ch, rc = OK;

	ch = wgetch(win[ACTIVE_W]);

	switch (ch) {
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
			break;
		case KEY_END:
			set_default_attr();
			content[ACTIVE_W].y_pos = content[ACTIVE_W].count;
			break;
		case KEY_F(4):
		case CTRL_O:
			rc = get_file_opt();
			break;
		case KEY_F(5):
		case CTRL_K:
			rc = get_dir_opt();
			break;
		case KEY_F(6):
		case CTRL_E:
			rc = get_extra_opt();
			break;
		case KEY_F(7):
		case CTRL_F:
			rc = get_help();
			break;
		case KEY_F(8):
		case CTRL_X:
			finalize();
			exit(EXIT_SUCCESS);
			break;
		case CTRL_G:
			rc = change_theme();
			break;
		case KEY_ENTER:
		case KEY_NL:
			rc = do_action();
			break;
		default:
			break;
	}

	return rc;
}
