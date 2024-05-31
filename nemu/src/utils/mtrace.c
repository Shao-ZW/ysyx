#include <stdio.h>
#include <common.h>

void mtrace_read(paddr_t addr) {
    printf("memory read:  "FMT_WORD"\n", addr);
}

void mtrace_write(paddr_t addr) {
    printf("memory write: "FMT_WORD"\n", addr);
}