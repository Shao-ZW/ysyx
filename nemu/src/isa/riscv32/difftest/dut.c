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
  bool flag = true;

  if(ref_r->pc != pc)
    flag = false; 

  for(int i = 0; i < NR_GPR; ++i)
    if(ref_r->gpr[check_reg_idx(i)] != gpr(i))
      flag = false;
  
  if(!flag) {
    printf("dut-pc : "FMT_PADDR"  ref-pc : "FMT_PADDR"\n", pc, ref_r->pc);
    for(int i = 0; i < NR_GPR; ++i)
      printf("dut-%-3s: "FMT_WORD"  ref-%-3s: "FMT_WORD"\n", reg_name(i), gpr(i), reg_name(i), ref_r->gpr[i]);
  }

  return flag;
}

void isa_difftest_attach() {
}
