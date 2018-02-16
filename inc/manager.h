#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "list.h"

#define CONFIG_FILE "/home/saboteur/Programming/github/YAFM/.config"
#define EDITOR_PATH "/home/saboteur/Programming/github/YATE/yate"

#define KEY_BS '\b'
#define KEY_CR '\r'
#define KEY_HT '\t'
#define KEY_NL '\n'

#define CTRL_A 1
#define CTRL_B 2
#define CTRL_C 3
#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_H 8
#define CTRL_I 9
#define CTRL_J 10
#define CTRL_K 11
#define CTRL_O 15
#define CTRL_W 23
#define CTRL_X 24
#define CTRL_Y 25

#define DEFPOS_Y 1
#define DEFPOS_X 1
#define NWINDOWS 3

WINDOW *win[NWINDOWS];

enum win_t { MENU_W, LEFT_W, RITE_W };
int ACTIVE_W;

char current_theme;
char copy_buffer[PATH_MAX + FILENAME_MAX];

struct win_cont_t {
	int x_pos;
	int y_pos;
	int count;
	int y_off;
	char path[PATH_MAX];
	struct list_t files;
};

struct win_cont_t content[NWINDOWS];

inline void __attribute__ ((always_inline))
dim_cursor(void)
{
	mvwchgat(win[ACTIVE_W], content[ACTIVE_W].y_pos,
		content[ACTIVE_W].x_pos, COLS / 2 - 2,
		A_DIM | A_REVERSE, 9, NULL);
}

int set_default_attr(void);

int fcmpr(const void *a, const void *b);
char **split_s(char *str, const char delim);

int display_content(enum win_t active);
int grab_files(enum win_t active);
int show_files(enum win_t active);

int perform_action(char *action);

int change_dir_to(char *name);
int create_file(char *name);
int edit_file(char *name);
int exec_prog(char *name);
int paste_file(void);
int remove_file(char *name);

inline void __attribute__ ((always_inline))
copy_file(char *name)
{
	sprintf(copy_buffer, "%s/%s",
		content[ACTIVE_W].path, ++name);
}

#endif
