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

#include "init.h"
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

#define CURSOR_CLR 9

WINDOW *win[NWINDOWS];

enum win_t { MENU_W, LEFT_W, RITE_W };

int ACTIVE_W;
extern char filename[FILENAME_MAX];

struct win_cont_t {
	int x_pos;
	int y_pos;
	int count;
	int y_off;
	char path[PATH_MAX];
	struct list_t files;
};

struct win_cont_t content[NWINDOWS];

void draw_window(enum win_t type);
void draw_menu(enum win_t type);
void draw_left(enum win_t type);
void draw_rite(enum win_t type);

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

int dim_cursor(void);
int set_default_attr(void);
int change_dir_to(char *dirname);

void finalize(void);
int exec_prog(char *progname);

#endif
