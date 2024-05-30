#include <string.h>
#include <stdio.h>
#include <cpu/decode.h>

struct{
  int start;
  int end;
  char buf[20][128];
} iringbuffer;

void init_iringbuffer() {
    iringbuffer.start = iringbuffer.end = 0;
}

void iringbuffer_write(const char* s) {
  strcpy(iringbuffer.buf[iringbuffer.end], s);
  iringbuffer.end = (iringbuffer.end + 1) % 20;
  if(iringbuffer.end == iringbuffer.start) {
    iringbuffer.start = (iringbuffer.start + 1) % 20;
  }
}

void iringbuffer_display() {
  int idx = iringbuffer.start;

  while(1){
    printf("%s", iringbuffer.buf[idx]);
    idx = (idx + 1) % 20;

    if(idx != iringbuffer.end) { putchar('\n'); }
    else { printf("  <--\n"); break; }
  }
}

void itrace(Decode *s) {
  char *p = s->logbuf;
  p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
  int ilen = s->snpc - s->pc;
  int i;
  uint8_t *inst = (uint8_t *)&s->isa.inst.val;
  for (i = ilen - 1; i >= 0; i --) {
    p += snprintf(p, 4, " %02x", inst[i]);
  }
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;

#ifndef CONFIG_ISA_loongarch32r
  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
      MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
#else
  p[0] = '\0'; // the upstream llvm does not support loongarch32r
#endif

  iringbuffer_write(s->logbuf);
}

