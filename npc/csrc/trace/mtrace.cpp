#include <stdio.h>

void mtrace_read(paddr_t addr) {
    printf("memory read  address : "FMT_WORD"\n", addr);
}

void mtrace_write(paddr_t addr) {
    printf("memory write address : "FMT_WORD"\n", addr);
}