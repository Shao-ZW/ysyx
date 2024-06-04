#ifndef __CPU_CPU_H__
#define __CPU_CPU_H__

#define RISCV_GPR_TYPE MUXDEF(CONFIG_RV64, uint64_t, uint32_t)
#define RISCV_GPR_NUM  MUXDEF(CONFIG_RVE , 16, 32)
#define DIFFTEST_REG_SIZE (sizeof(RISCV_GPR_TYPE) * (RISCV_GPR_NUM + 1)) // GPRs + pc

typedef struct {
  vaddr_t pc;
  vaddr_t npc;
  word_t gpr[RISCV_GPR_NUM];
  word_t inst_val;
  IFDEF(CONFIG_ITRACE, char logbuf[128]);
} CPU_state;

void reg_display();
void cpu_exec(uint64_t n);

#endif
