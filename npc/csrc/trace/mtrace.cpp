#include "common.h"

void mtrace_read(paddr_t addr, uint32_t rdata) {
    printf(ANSI_FMT("memory read address : " FMT_PADDR "  read data : " 
            FMT_WORD "\n", ANSI_FG_YELLOW), addr, rdata);
}

void mtrace_write(paddr_t addr, uint32_t wdata, char wmask) {
    printf(ANSI_FMT("memory write address : " FMT_PADDR "  write data : " 
            FMT_WORD "  write mask : " FMT_WORD "\n", ANSI_FG_YELLOW), addr, wdata, wmask);
}
