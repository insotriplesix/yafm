/********************************************************************
 * PROGRAM: yafm
 * FILE: manager.h
 * PURPOSE: main file manager functionality
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

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

/*
 * This FM implementation has 3 windows: menu, left and right.
 *
 * +-----------------------+
 * | (1) Menu window       |
 * +-----------------------+
 * |           |           |
 * | (2) Left  | (3) Right |
 * |           |           |
 * +-----------------------+
 *
 * Menu window contains the set of F(k) keys that you can use and get
 * the manager functionality. For example, if you press the 'Help'
 * button, you`ll get the full list of control and func keys that you
 * can use in the program. The other windows display the content of its
 * current directory.
 */

#define NWINDOWS 3

/*
 * Each window framed, then default relative position of cursor will be
 * (y:1, x:1). X-axis limit for left & right windows: number of columns
 * / 2 - 2 (left border and the middle one). Y-axis limit: number of
 * lines - 5 (3 of menu window + top & bottom borders).
 */

#define DEFPOS_Y 1
#define DEFPOS_X 1
#define LIMIT_X (COLS / 2 - 2)
#define LIMIT_Y (LINES - 5)

/* Stores a path to the .config file */
char CONFIG_PATH[PATH_MAX + FILENAME_MAX + 1];

/* Stores a path to the editor`s binary file */
char EDITOR_PATH[PATH_MAX + FILENAME_MAX + 1];

/* File manager windows */
WINDOW *win[NWINDOWS];

/* Window types */
enum win_t { MENU_W, LEFT_W, RITE_W };

/* Selected window (left or right) */
int ACTIVE_W;

/* Current theme used in the manager */
char current_theme;

/* Used for 'copy_file' option (absolute file path) */
char copy_buffer[PATH_MAX + FILENAME_MAX + 1];

/* Stores the default terminal attributes */
struct termios term_attr;

/* The main structure that contains all necessary manager data */
struct win_cont_t {
	int x_pos;  // X-axis cursor position
	int y_pos;  // Y-axis cursor position
	int y_off;  // rows offset
	int count;  // number of rows (files in dir)
	char path[PATH_MAX]; // cwd
	struct list_t files; // list of files
};

struct win_cont_t content[NWINDOWS];

/* Terminal modes */
void enable_raw_mode(void);
void disable_raw_mode(void);

/* Display functions */
int display_content(enum win_t active);
int grab_files(enum win_t active);
int show_files(enum win_t active);

void dim_cursor(void);
void set_default_attr(void);

/* Actions */
void perform_action(char *action);

/* Files actions */
void copy_file(char *name);
void create_file(void);
void create_file_from_buf(char *name);
void edit_file(char *name);
void exec_prog(char *name);
void paste_file(void);
void remove_file(char *name);

/* Dir actions */
void change_dir_to(char *name);
void make_dir(void);
void remove_dir(char *name);

/* Other */
void get_config(void);
void load_config(void);
void save_config(void);

void init_content(void);

int fcmpr(const void *a, const void *b);
void get_editor_path(void);

#endif
