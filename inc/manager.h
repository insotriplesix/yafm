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
#include <termios.h>
#include <unistd.h>

#include "list.h"

#define CONFIG_FILE "/home/saboteur/Programming/github/YAFM/.config"
#define EDITOR_PATH "/home/saboteur/Programming/github/YATE/yate"

#define DEFPOS_Y 1
#define DEFPOS_X 1
#define NWINDOWS 3

WINDOW *win[NWINDOWS];

enum win_t { MENU_W, LEFT_W, RITE_W };
int ACTIVE_W;

char current_theme;
char copy_buffer[PATH_MAX + FILENAME_MAX];

struct termios term_attr;

struct win_cont_t {
	int x_pos;
	int y_pos;
	int count;
	int y_off;
	char path[PATH_MAX];
	struct list_t files;
};

struct win_cont_t content[NWINDOWS];

void enable_raw_mode(void);
void disable_raw_mode(void);

void dim_cursor(void);
int set_default_attr(void);

int fcmpr(const void *a, const void *b);

int display_content(enum win_t active);
int grab_files(enum win_t active);
int show_files(enum win_t active);

int perform_action(char *action);

int change_dir_to(char *name);
int make_dir(char *name);
int remove_dir(char *name);

void copy_file(char *name);
int create_file(char *name);
int create_file_from_buf(char *name);
int edit_file(char *name);
int exec_prog(char *name);
int paste_file(void);
int remove_file(char *name);

#endif
