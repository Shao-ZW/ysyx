#include <verilated.h>
#include "verilated_vcd_c.h"
#include "Vtop.h"
#include "Vtop___024root.h"
#include "common.h"
#include "cpu.h"

VerilatedContext* contextp = nullptr;
VerilatedVcdC* vcd = nullptr;
Vtop* top = nullptr;
vluint64_t cur_time = 0;

void npc_eval(int clk, int rst = 0) {
  top->rst = rst;
  top->clk = clk;
  top->eval();
  vcd->dump(cur_time);
  cur_time++;
}

void cpu_update() {
  cpu.pc  = top->rootp->top__DOT__u_CPU__DOT__u_PC__DOT__pc_reg;
  cpu.npc = top->rootp->top__DOT__u_CPU__DOT__next_pc;

  for(int i = 0; i < RISCV_GPR_NUM; ++i) {
    cpu.gpr[i] = top->rootp->top__DOT__u_CPU__DOT__u_regfile__DOT__reg_array[i];
  }
  cpu.inst_val = top->rootp->top__DOT__inst;
}

static void restart() {
  /* Synchronous reset */
  npc_eval(0, 1);
  npc_eval(1, 1);
  cpu_update();
}

void init_sim() {
  contextp = new VerilatedContext;
  top = new Vtop{contextp};
  vcd = new VerilatedVcdC;

  Verilated::traceEverOn(true);
  top->trace(vcd, 0);
  vcd->open("./build/wave.vcd");

  restart();
}

void finish_sim() {
  #ifdef CONFIG_FTRACE
  void ftrace_display();
  ftrace_display();
  #endif
  
  top->final();
  vcd->close();
}

