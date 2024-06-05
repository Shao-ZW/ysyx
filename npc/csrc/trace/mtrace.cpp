#include "common.h"

void mtrace_read(paddr_t addr, uint32_t rdata) {
    printf(ANSI_FMT("memory raddr : " FMT_PADDR "  rdata : " 
            FMT_WORD "\n", ANSI_FG_YELLOW), addr, rdata);
}

void mtrace_write(paddr_t addr, uint32_t wdata, char wmask) {
    printf(ANSI_FMT("memory waddr : " FMT_PADDR "  wdata : " 
            FMT_WORD "  wmask : 0b%x\n", ANSI_FG_YELLOW), addr, wdata, wmask);
}
