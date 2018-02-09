#ifndef __MANAGER_H__
#define __MANAGER_H__

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

#define KEY_BS '\b'
#define KEY_CR '\r'
#define KEY_HT '\t'
#define KEY_NL '\n'

#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_K 11
#define CTRL_O 15
#define CTRL_X 24
#define CTRL_Y 25

#define DEFPOS_Y 1
#define DEFPOS_X 1
#define NWINDOWS 3

WINDOW *win[NWINDOWS];

enum win_t { MENU_W, LEFT_W, RITE_W };
int ACTIVE_W;

struct win_cont_t {
	int x_pos;
	int y_pos;
	int count;
	int y_off;
	char path[PATH_MAX];
	struct list_t files;
};

struct win_cont_t content[NWINDOWS];

void dim_cursor(void);
int set_default_attr(void);

int fcmpr(const void *a, const void *b);

int display_content(enum win_t active);
int grab_files(enum win_t active);
int show_files(enum win_t active);

int change_dir_to(char *name);
int edit_file(char *name);
int exec_prog(char *name);

#endif
