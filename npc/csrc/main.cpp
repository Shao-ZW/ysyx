#include "VCPU.h"
#include "verilated_vcd_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

vluint64_t cur_time = 0;
VCPU* cpu;
VerilatedContext* contextp;

void single_cycle() {
  cpu->clock = 0; cpu->eval();
  cpu->clock = 1; cpu->eval();
}

int main(int argc, char** argv) {
  contextp = new VerilatedContext;
  cpu = new VCPU{contextp};

  while(1) {
    single_cycle();
    cur_time++;
  }

  cpu->final();
  delete cpu;

  return 0;
}
