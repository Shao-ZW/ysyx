#include "verilated_vcd_c.h"
#include "Vtop.h"
#include <verilated.h>

std::unique_ptr<Vtop> top;
std::unique_ptr<VerilatedVcdC> vcd;
std::unique_ptr<VerilatedContext> contextp;
vluint64_t cur_time = 0;

void eval(int clk, int rst = 0) {
  top->rst = rst;
  top->clk = clk;
  top->eval();
  vcd->dump(cur_time);
  cur_time++;
}

void init_sim() {
  contextp = std::make_unique<VerilatedContext>();
  //contextp->commandArgs(0, nullptr); // 可根据实际情况传递命令行参数
  top = std::make_unique<Vtop>(contextp.get());
  vcd = std::make_unique<VerilatedVcdC>();

  // Verilated::traceEverOn(true);
  // top->trace(vcd.get(), 0);
  // vcd->open("./build/wave.vcd");
  // printf("120");
}

void finish_sim() {
  //IFDEF(CONFIG_FTRACE, ftrace_display());
  
  // top->final();
  // vcd->close();
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