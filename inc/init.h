#ifndef __INIT_H__
#define __INIT_H__

#include "manager.h"

#define restore_windows() (initialize(0))

void initialize(int is_first);
void finalize(void);

int init_colors(void);
int init_content(void);
int init_gui(void);
int init_ncurses(void);
int init_windows(void);

int get_config(void);
int load_config(void);
int save_config(void);

#endif
