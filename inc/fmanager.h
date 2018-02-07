#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

#define NWINDOWS 3

WINDOW *win[NWINDOWS];

enum win_t { MENU_W, LEFT_W, RITE_W };

int ACTIVE_W;
extern char filename[FILENAME_MAX];

struct win_cont_t {
	int x_pos;
	int y_pos;
	int count;
	char path[PATH_MAX];
	struct list_t files;
};

struct win_cont_t content[NWINDOWS];

int change_theme(void);
int change_theme_popup(void);

int get_extra(void);
int get_help(void);

int open_file(char **buf, int *size, bool from_arg);
int open_file_popup(void);

int save_file(char *buf, int size);
int save_file_popup(void);

int grab_files(enum win_t active);
int show_files(enum win_t active);
int display_content(enum win_t active);

#endif
