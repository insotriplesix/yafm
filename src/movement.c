/********************************************************************
 * PROGRAM: yafm
 * FILE: movement.c
 * PURPOSE: arrow keys handlers
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#include "movement.h"

/*
 * Function: move_left
 * -------------------
 * Description:
 *  Moves the cursor to the left window.
 */

void
move_left(void)
{
	if (ACTIVE_W == RITE_W) {
		dim_cursor();
		ACTIVE_W = LEFT_W;
	}
}

/*
 * Function: move_right
 * --------------------
 * Description:
 *  Moves the cursor to the right window.
 */

void
move_right(void)
{
	if (ACTIVE_W == LEFT_W) {
		dim_cursor();
		ACTIVE_W = RITE_W;
	}
}

/*
 * Function: move_up
 * -----------------
 * Description:
 *  Moves the cursor up to the previous row.
 */

void
move_up(void)
{
	if (content[ACTIVE_W].y_pos - 1 > 0) {
		set_default_attr();
		content[ACTIVE_W].y_pos--;
	} else if (content[ACTIVE_W].y_off) {
		content[ACTIVE_W].y_off--;
		wclear(win[ACTIVE_W]);
		draw_window(ACTIVE_W);
		display_content(ACTIVE_W);
	}
}

/*
 * Function: move_down
 * -------------------
 * Description:
 *  Moves the cursor down to the next row.
 */

void
move_down(void)
{
	if ((content[ACTIVE_W].y_pos < LINES - 5) &&
		(content[ACTIVE_W].y_pos + 1 < content[ACTIVE_W].count + 1)) {
		set_default_attr();
		content[ACTIVE_W].y_pos++;
	} else if (content[ACTIVE_W].y_pos + 1 == LINES - 4 &&
		content[ACTIVE_W].y_off < content[ACTIVE_W].count - LINES + 5) {
		content[ACTIVE_W].y_off++;
		wclear(win[ACTIVE_W]);
		draw_window(ACTIVE_W);
		display_content(ACTIVE_W);
	}
}

/*
 * Function: move_home
 * -------------------
 * Description:
 *  Moves the cursor up to the first row on a screen.
 */

void
move_home(void)
{
	set_default_attr();
	content[ACTIVE_W].y_pos = DEFPOS_Y;
}

/*
 * Function: move_end
 * ------------------
 * Description:
 *  Moves the cursor down to the last row on a screen.
 */

void
move_end(void)
{
	set_default_attr();
	content[ACTIVE_W].y_pos = content[ACTIVE_W].count;
}
