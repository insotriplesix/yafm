#ifndef __READKEY_H__
#define __READKEY_H__

#include "gui.h"
#include "init.h"
#include "manager.h"
#include "movement.h"

#define KEY_NL '\n'

#define CTRL_C 3
#define CTRL_D 4
#define CTRL_E 5
#define CTRL_F 6
#define CTRL_G 7
#define CTRL_I 9
#define CTRL_K 11
#define CTRL_M 13
#define CTRL_Q 17
#define CTRL_R 18
#define CTRL_V 22
#define CTRL_W 23
#define CTRL_X 24

int readkey(void);

#endif
