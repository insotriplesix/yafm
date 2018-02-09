#ifndef __INIT_H__
#define __INIT_H__

#include "manager.h"

#define restore_windows() initialize()

void finalize(void);
void initialize(void);

int init_colors(void);
int init_gui(void);
int init_ncurses(void);
int init_windows(void);

#endif
