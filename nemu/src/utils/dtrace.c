#include <stdio.h>
#include <common.h>

void dtrace_read(const char *name) {
    printf(ANSI_FMT("%-10s read\n", ANSI_FG_YELLOW), name);
}

void dtrace_write(const char *name) {
    printf(ANSI_FMT("%-10s write\n", ANSI_FG_YELLOW), name);
}