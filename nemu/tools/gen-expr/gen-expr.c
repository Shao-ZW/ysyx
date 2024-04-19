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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define NBUF 65536

static char buf[NBUF] = {};
static char code_buf[NBUF + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int idx = 0;

static void gen(char ch, int *success) {
  if(idx >= NBUF) {
    *success = 0;
    return;
  }

  buf[idx++] = ch;
}

static void gen_rand_num(int *success) {
  int num = rand() % 50;
  int digit[10], t = 0;

  while(num) {
    digit[t++] = num % 10;
    num /= 10;
  }

  while(t > 0) 
    gen(digit[--t] + '0', success);
}

static void gen_rand_op(int *success) {
  switch (rand() % 7) {
    case 0: 
      gen('+', success);
      break;
    case 1:
      gen('-', success);
      break;
    case 2:
      gen('*', success);
      break;
    case 3:
      gen('/', success);
      break;
    case 4:
      gen('&', success); gen('&', success);
      break;
    case 5:
      gen('=', success); gen('=', success);
      break;
    case 6:
      gen('!', success); gen('=', success);
      break;
  }
}

static void gen_rand_expression(int *success) {
  switch (rand() % 3) {
    case 0: gen_rand_num(success); break;
    case 1: gen('(', success); gen_rand_expression(success); gen(')', success); break;
    default: gen_rand_expression(success); gen_rand_op(success); gen_rand_expression(success); break;
  }
}

static void gen_rand_expr(int *success) {
  idx = 0;
  gen_rand_expression(success);
  gen('\0', success);
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    int success = 1;
    gen_rand_expr(&success);

    if(!success) {
      i--;
      continue;
    }
      
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Wall -Werror /tmp/.code.c -o /tmp/.expr 2>> error.log");
    if (ret != 0) {
      i--;
      continue;
    }
    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    unsigned result;
    ret = fscanf(fp, "%u", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
