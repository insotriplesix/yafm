#ifndef __INIT_H__
#define __INIT_H__

#include "fmanager.h"

#define restore_windows() (initialize())

void initialize();

int init_ncurses();
int init_colors();
int init_windows();

#endif
