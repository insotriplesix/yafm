#ifndef __GUI_H__
#define __GUI_H__

#include "manager.h"

#define MENU_CLR	COLOR_PAIR(1)
#define EDIT_CLR	COLOR_PAIR(2)
#define POPUP_CLR	COLOR_PAIR(3)
#define BORDER_CLR	COLOR_PAIR(4)
#define DIR_CLR		COLOR_PAIR(5)
#define REG_CLR		COLOR_PAIR(6)
#define EXE_CLR		COLOR_PAIR(7)
#define SYM_CLR		COLOR_PAIR(8)
#define CURSOR_CLR	COLOR_PAIR(9)

enum color_t {
	NONE_C, MENU_C, EDIT_C, POPUP_C, BORDER_C,
	DIR_C, REG_C, EXE_C, SYM_C, CURSOR_C
};

void draw_left(enum win_t wtype);
void draw_menu(enum win_t wtype);
void draw_rite(enum win_t wtype);
void draw_window(enum win_t wtype);

int change_theme(void);
int change_theme_popup(void);

int get_dir_opt(void);
int get_file_opt(void);
int get_extra_opt(void);

int get_help(void);

inline void __attribute__ ((always_inline))
update_gui(void)
{
	for (int i = 0; i < NWINDOWS; ++i) {
		touchwin(win[i]);
		wnoutrefresh(win[i]);
	}

	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, COLS / 2 - 2, A_NORMAL,
		CURSOR_C, NULL);

	doupdate();
}

#endif
