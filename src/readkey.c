/********************************************************************
 * PROGRAM: yafm
 * FILE: readkey.c
 * PURPOSE: user input processing
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#include "readkey.h"

/*
 * Function: readkey
 * -----------------
 * Description:
 *  User input handler routine.
 */

void
readkey(void)
{
	int ch = wgetch(win[ACTIVE_W]);

	switch (ch) {
		// Movements
		case KEY_LEFT:		move_left();		break;
		case KEY_RIGHT:		move_right();		break;
		case KEY_UP:		move_up();			break;
		case KEY_DOWN:		move_down();		break;
		case KEY_HOME:		move_home();		break;
		case KEY_END:		move_end();			break;
		// Files manip
		case KEY_F(4):
		case CTRL_F:		get_file_opt();		break;
		case KEY_ENTER:
		case KEY_NL:		perform_action("edit_file");	break;
		case CTRL_Q:		perform_action("create_file");	break;
		case CTRL_C:		perform_action("copy_file");	break;
		case CTRL_V:		perform_action("paste_file");	break;
		case CTRL_R:		perform_action("remove_file");	break;
		// Dir manip
		case KEY_F(5):
		case CTRL_D:		get_dir_opt();		break;
		case CTRL_W:		perform_action("make_dir");		break;
		case CTRL_K:		perform_action("remove_dir");	break;
		// Other
		case CTRL_G:		change_theme(1);	break;
		case KEY_F(6):
		case CTRL_E:		get_extra_opt();	break;
		case KEY_F(7):
		case CTRL_I:		get_help();			break;
		case KEY_F(8):
		case CTRL_X:
			finalize(1);
			exit(EXIT_SUCCESS);
		default: break;
	}
}
