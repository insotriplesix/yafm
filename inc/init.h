/********************************************************************
 * PROGRAM: yafm
 * FILE: init.h
 * PURPOSE: manager initialization functions
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#ifndef __INIT_H__
#define __INIT_H__

#include "manager.h"

#define restore_windows()		\
	do {						\
		finalize(0);			\
		initialize(0);			\
	} while (0);

void initialize(int conf);
void finalize(int conf);

int init_colors(void);
void init_gui(void);
int init_ncurses(void);
void init_windows(void);

#endif
