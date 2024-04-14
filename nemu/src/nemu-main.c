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

#include <common.h>
//#include <unistd.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
// word_t expr(char*, bool*);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /*Test expr()*/
  /*
  FILE *fp = fopen("tools/gen-expr/build/input.txt", "r");
  assert(fp != NULL);

  char expression[65536];
  unsigned right_ans;
  for(int i = 0; i < 1000; ++i) {
    int t = fscanf(fp, "%u %s", &right_ans, expression);
    assert(t == 2);
    bool success = true;
    word_t res = expr(expression, &success);
    
    if(!success || (res != right_ans)) 
      printf("%d fali: flag: %d my_res:%u right_ans:%u expr:%s\n", i, success, res, right_ans, expression);      
    else  
      printf("%d ok\n", i);
  }
  return 0;
  */
  
  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}