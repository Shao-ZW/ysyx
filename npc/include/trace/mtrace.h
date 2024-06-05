#ifndef __MTRACE_H__
#define __MTRACE_H__

#include "common.h"

void mtrace_read(paddr_t addr, uint32_t rdata);
void mtrace_write(paddr_t addr, uint32_t wdata, char wmask);

#endif