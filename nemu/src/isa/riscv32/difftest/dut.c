/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

#define NR_GPR MUXDEF(CONFIG_RVE, 16, 32)

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  // if(ref_r->pc != pc) {
  //   printf("ref-pc  %x   dut-pc  %x\n", ref_r->pc, pc);
  //   return false;
  // }
  printf("%x %x\n", ref_r->pc, pc);
  for(int i = 0; i < NR_GPR; ++i)
    if(ref_r->gpr[check_reg_idx(i)] != gpr(i))
      return false;


  return true;
}

void isa_difftest_attach() {
}
