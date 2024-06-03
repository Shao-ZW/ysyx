#include "common.h"

void mtrace_read(paddr_t addr) {
    printf(ANSI_FMT("memory read  address : " FMT_WORD "\n", ANSI_FG_YELLOW), addr);
}

void mtrace_write(paddr_t addr) {
    printf(ANSI_FMT("memory write address : " FMT_WORD "\n", ANSI_FG_YELLOW), addr);
}
