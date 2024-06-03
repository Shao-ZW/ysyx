#ifndef __CPU_CPU_H__
#define __CPU_CPU_H__

#define GPR_NUM MUXDEF(CONFIG_RVE, 16, 32)

typedef struct {
  vaddr_t pc;
  word_t gpr[GPR_NUM];
  word_t inst_val;
  IFDEF(CONFIG_ITRACE, char logbuf[128]);
} CPU_state;

void reg_display();

#endif
