#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "gui.h"
#include "manager.h"

inline void
__attribute__ ((always_inline))
move_left(void)
{
	if (ACTIVE_W == RITE_W) {
		dim_cursor();
		ACTIVE_W = LEFT_W;
	}
}

inline void
__attribute__ ((always_inline))
move_right(void)
{
	if (ACTIVE_W == LEFT_W) {
		dim_cursor();
		ACTIVE_W = RITE_W;
	}
}

inline void
__attribute__ ((always_inline))
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

inline void
__attribute__ ((always_inline))
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

#endif
