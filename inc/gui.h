/********************************************************************
 * PROGRAM: yafm
 * FILE: gui.h
 * PURPOSE: manager gui
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#ifndef __GUI_H__
#define __GUI_H__

#include "manager.h"

#define MENU_CLR COLOR_PAIR(1)
#define EDIT_CLR COLOR_PAIR(2)
#define POPUP_CLR COLOR_PAIR(3)
#define BORDER_CLR COLOR_PAIR(4)
#define DIR_CLR COLOR_PAIR(5)
#define REG_CLR COLOR_PAIR(6)
#define EXE_CLR COLOR_PAIR(7)
#define SYM_CLR COLOR_PAIR(8)
#define CURSOR_CLR COLOR_PAIR(9)

#define repaint_window(wtype) (draw_window(wtype))

enum color_t {
	NONE_C, MENU_C, EDIT_C, POPUP_C, BORDER_C,
	DIR_C, REG_C, EXE_C, SYM_C, CURSOR_C
};

void update_gui(void);

void draw_window(enum win_t wtype);
void draw_left(enum win_t wtype);
void draw_rite(enum win_t wtype);
void draw_menu(enum win_t wtype);

void change_theme(int popup);
int change_theme_popup(void);

void get_dir_opt(void);
void get_file_opt(void);
void get_extra_opt(void);
void get_help(void);

void create_file_popup(char *fname);
void make_dir_popup(char *dname);

#endif
