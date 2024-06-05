#ifndef __FTRACE_H__
#define __FTRACE_H__

#include "common.h"

void ftrace_add(int type, vaddr_t func_addr, vaddr_t inst_addr);
void ftrace_display();

#endif