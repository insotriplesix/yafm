#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "fmanager.h"

inline void __attribute__ ((always_inline))
move_left(void)
{
	if (ACTIVE_W == RITE_W) {
		wmove(win[LEFT_W], content[LEFT_W].y_pos, content[LEFT_W].x_pos);
		ACTIVE_W = LEFT_W;
	}
}

inline void __attribute__ ((always_inline))
move_right(void)
{
	if (ACTIVE_W == LEFT_W) {
		wmove(win[RITE_W], content[RITE_W].y_pos, content[RITE_W].x_pos);
		ACTIVE_W = RITE_W;
	}
}

inline void __attribute__ ((always_inline))
move_up(int border)
{
	if (content[ACTIVE_W].y_pos - 1 > border) {
		content[ACTIVE_W].y_pos--;
		wmove(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos);
	}
}

inline void __attribute__ ((always_inline))
move_down(int border)
{
	if (content[ACTIVE_W].y_pos + 1 < border) {
		content[ACTIVE_W].y_pos++;
		wmove(win[ACTIVE_W], content[ACTIVE_W].y_pos, content[ACTIVE_W].x_pos);
	}
}

#endif
