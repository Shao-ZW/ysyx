#include "verilated_vcd_c.h"
#include "Vtop.h"
#include <verilated.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

vluint64_t cur_time = 0;

int main(int argc, char **argv) {
  const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};

  contextp->commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  const std::unique_ptr<Vtop> top{new Vtop{contextp.get(), "TOP"}};
  const std::unique_ptr<VerilatedVcdC> vcd{new VerilatedVcdC};
  
  top->trace(vcd, 0);
  vcd->open("./build/wave.vcd");

  top->rst = 1;



  top->rst = 0;

  while(cur_time < 20 && !Verilated::gotFinish()) {

    top->eval();
    vcd->dump(cur_time);
  }

  top->final();
  vcd->close();

  return 0;
}
