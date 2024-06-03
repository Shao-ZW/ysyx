#include "common.h"
#include "cpu/cpu.h"

extern CPU_state cpu;

struct{
  int start;
  int end;
  char buf[20][128];
} iringbuffer = {0, 0, {{0}}};

void iringbuffer_write(const char* s) {
  strcpy(iringbuffer.buf[iringbuffer.end], s);
  iringbuffer.end = (iringbuffer.end + 1) % 20;
  if(iringbuffer.end == iringbuffer.start) {
    iringbuffer.start = (iringbuffer.start + 1) % 20;
  }
}

void iringbuffer_display() {
  printf(ANSI_FMT("IRINGBUFFER", ANSI_FG_YELLOW)":\n");

  int idx = iringbuffer.start;
  while(1){
    printf("%s", iringbuffer.buf[idx]);
    idx = (idx + 1) % 20;

    if(idx != iringbuffer.end) { putchar('\n'); }
    else { printf("  <--\n"); break; }
  }
}

extern "C" void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

void itrace() {
#ifdef CONFIG_ITRACE
  char *p = cpu.logbuf;
  p += snprintf(p, sizeof(cpu.logbuf), FMT_WORD ":", cpu.pc);
  int ilen = 4;
  int i;
  uint8_t *inst = (uint8_t *)&cpu.inst_val;
  for (i = ilen - 1; i >= 0; i --) {
    p += snprintf(p, 4, " %02x", inst[i]);
  }
  int ilen_max = 4;
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;

  disassemble(p, cpu.logbuf + sizeof(cpu.logbuf) - p, cpu.pc, (uint8_t *)&cpu.inst_val, ilen);
  
  iringbuffer_write(cpu.logbuf);
#endif
}
