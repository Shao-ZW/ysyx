#include "common.h"
#include "cpu/cpu.h"
#include "Vtop.h"
#include "Vtop___024root.h"

extern CPU_state cpu;
extern Vtop* top;

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

word_t reg_str2val(const char *s) {
  for(int i = 0; i < RISCV_GPR_NUM; ++i) {
    if(strcmp(regs[i], s) == 0) {
      return cpu.gpr[i];
    }
  }
  return -1;
}

void reg_display() {
  for(int i = 0; i < RISCV_GPR_NUM; ++i) {
    word_t val = reg_str2val(regs[i]);
    printf("%-5s " FMT_WORD "   " FMT_UNSIGN "\n", regs[i], val, val);
  }
}

void cpu_update() {
  cpu.pc = top->rootp->top__DOT__u_CPU__DOT__u_PC__DOT__pc_reg;
  for(int i = 0; i < RISCV_GPR_NUM; ++i) {
    cpu.gpr[i] = top->rootp->top__DOT__u_CPU__DOT__u_regfile__DOT__reg_array[i];
  }
  cpu.inst_val = top->rootp->top__DOT__inst;
}