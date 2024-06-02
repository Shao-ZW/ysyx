#include "verilated_vcd_c.h"
#include "Vtop.h"
#include <verilated.h>
#include <stdio.h>

VerilatedContext* contextp = nullptr;
VerilatedVcdC* vcd = nullptr;
Vtop* top = nullptr;
vluint64_t cur_time = 0;

void eval(int clk, int rst = 0) {
  top->rst = rst;
  top->clk = clk;
  top->eval();
  vcd->dump(cur_time);
  printf("%d", (int)cur_time);
  cur_time++;
}

void init_sim() {
  contextp = new VerilatedContext;
  top = new Vtop{contextp};
  vcd = new VerilatedVcdC;

  Verilated::traceEverOn(true);
  top->trace(vcd, 0);
  vcd->open("../build/wave.vcd");
}

void finish_sim() {
  //IFDEF(CONFIG_FTRACE, ftrace_display());
  
  top->final();
  vcd->close();
}

void restart() {
  // Synchronous reset
  eval(0, 1);
  eval(1, 1);
}

void exec() {
  eval(0);
  eval(1);
}