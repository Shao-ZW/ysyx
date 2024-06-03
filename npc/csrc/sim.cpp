#include <verilated.h>
#include "verilated_vcd_c.h"
#include "Vtop.h"

VerilatedContext* contextp = nullptr;
VerilatedVcdC* vcd = nullptr;
Vtop* top = nullptr;
vluint64_t cur_time = 0;

void npc_eval(int clk, int rst = 0) {
  top->rst = rst;
  top->clk = clk;
  if(clk != 0)  // should eval() but for the mtrace act normallly
    top->eval();
  vcd->dump(cur_time);
  cur_time++;
}

static void restart() {
  /* Synchronous reset */
  npc_eval(0, 1);
  npc_eval(1, 1);
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
