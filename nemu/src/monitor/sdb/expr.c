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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/vaddr.h>

#define IS_BIN_OPERATOR(type) (type == TK_AND || type == TK_EQ || type == TK_NEQ || type == '+' || type == '-' || type == '*' || type == '/')
#define IS_SIG_OPERATOR(type) (type == TK_DEREF || type == TK_POS || type == TK_NEG)
#define NTOKEN 256

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NEQ,
  TK_DEC, TK_BIN, TK_HEX,
  TK_REG, TK_AND, TK_DEREF, TK_POS, TK_NEG
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {"0[Xx][0-9a-fA-F]+", TK_HEX},          // hexadecimal integer
  {"0[Bb][01]+", TK_BIN},                 // binary integer
  {"[0-9]+", TK_DEC},                     // decimal integer
  {"\\$[\\$a-z][0-9]{1,2}", TK_REG},      // access reg
  {" +", TK_NOTYPE},                      // spaces
  {"\\+", '+'},                           // plus | plus sign
  {"\\-", '-'},                           // sub | minus sign
  {"\\*", '*'},                           // mul | dereference
  {"\\/", '/'},                           // div
  {"\\(", '('},                           // left paren
  {"\\)", ')'},                           // right paren
  {"==", TK_EQ},                          // equal
  {"!=", TK_NEQ},                         // not equal
  {"&&", TK_AND}                          // and
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[NTOKEN] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;

          case TK_DEC: case TK_BIN: case TK_HEX: case TK_REG:
            assert(substr_len < 32);
            assert(nr_token < NTOKEN);
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, e + position, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;

          default: 
            assert(nr_token < NTOKEN);
            tokens[nr_token++].type = rules[i].token_type;
            break;
        }

        position += substr_len;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static word_t str_to_num(char *str, bool *success) {
  word_t val = 0;
  char *endptr;

  if(str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
    val = strtol(str + 2, &endptr, 16);
  else if(str[0] == '0' && (str[1] == 'b' || str[1] == 'B'))
    val = strtol(str + 2, &endptr, 2);
  else 
    val = strtol(str, &endptr, 10);
    
  if(*endptr != '\0') {
    *success = false;
    return 0;
  }

  return val;
}

static word_t access_reg(char *str, bool *success) {
  assert(str[0] == '$');
  word_t val = isa_reg_str2val(str + 1, success);
  return val;
}

/*
priority
  *        deference
  +  -     sign
  /  *     div and mul
  +  -     add and sub
  == !=   equal and not equal
  &&       and
*/

// Return the priority of the binary operators
static int get_priority(int type) {
  switch (type) {
    case TK_AND:
      return 0;
    case TK_EQ: case TK_NEQ:
      return 1;
    case '+': case '-':
      return 2;
    case '*': case '/':
      return 3;
    default:
      assert(0);
  }

  return 0;
}

// Find the operator with the lowest precedence in the expression
static int get_op(int l, int r, bool *success) {
  // Prioritize the search for binary operators

  int p = l, op = -1;
  int cnt_leftparen = 0;

  // Ignore operators within parentheses
  for(int i = l; i <= r; ++i) {
    int type = tokens[i].type;

    if(type == '(') {
      cnt_leftparen++;
    }
    else if(type == ')') {
      cnt_leftparen--;
    }
    else if(IS_BIN_OPERATOR(type)) {
      if(cnt_leftparen == 0 && (op == -1 || get_priority(type) <= get_priority(op))) {
        p = i;
        op = type;
      }
    }
  }

  //If there are no binary operators in the expression
  if(op == -1 && IS_SIG_OPERATOR(tokens[l].type))
    p = l;
  else if(op == -1) 
    *success = false;

  return p;
}

static bool check_parentheses(int l, int r) {
  if(tokens[l].type != '(' || tokens[r].type != ')')
    return false;

  int cnt_leftparen = 0;

  for(int i = l; i < r; ++i) {
    if(tokens[i].type == '(')
      cnt_leftparen++;
    else if(tokens[i].type == ')')
      cnt_leftparen--;
      
    if(cnt_leftparen == 0)
      return false;
  }

  return true;
}

static word_t eval(int l, int r, bool *success) {
  if (l > r) {
    *success = false;
    return 0;
  }
  else if (l == r) {
    if(tokens[l].type != TK_BIN && tokens[l].type != TK_DEC && tokens[l].type != TK_HEX && tokens[l].type != TK_REG) {
      *success = false;
      return 0;
    }
    return (tokens[l].type == TK_REG ? access_reg(tokens[l].str, success) : str_to_num(tokens[l].str, success));
  }
  else if (check_parentheses(l, r)) {
    return eval(l + 1, r - 1, success);
  }
  else {
    int op = get_op(l, r, success);
  
    if(op == l) { // unary operator
      word_t val = eval(op + 1, r, success);
      switch (tokens[op].type) {
        case TK_POS: 
          return val;
        case TK_NEG: 
          return -val;
        case TK_DEREF: 
          return vaddr_read(val, 4);
        default: 
          assert(0);
      }
    }
    else {
      word_t val1 = eval(l, op - 1, success);
      word_t val2 = eval(op + 1, r, success);

      switch (tokens[op].type) {
        case '+': 
          return val1 + val2;
        case '-': 
          return val1 - val2;
        case '*': 
          return val1 * val2;
        case '/': 
          return val1 / val2;
        case TK_AND : 
          return val1 && val2;
        case TK_EQ : 
          return val1 == val2;
        case TK_NEQ : 
          return val1 != val2;
        default: 
          assert(0);
      }
    }
  }

  return 0;
}

static void token_processing() {
  for (int i = 0; i < nr_token; i++) {
    if (tokens[i].type == '*' && (i == 0 || IS_BIN_OPERATOR(tokens[i - 1].type) ||  IS_SIG_OPERATOR(tokens[i - 1].type) || tokens[i - 1].type == '(')) {
      tokens[i].type = TK_DEREF;
    }
    else if (tokens[i].type == '+' && (i == 0 || IS_BIN_OPERATOR(tokens[i - 1].type) ||  IS_SIG_OPERATOR(tokens[i - 1].type) || tokens[i - 1].type == '(')) {
      tokens[i].type = TK_POS;
    }
    else if (tokens[i].type == '-' && (i == 0 || IS_BIN_OPERATOR(tokens[i - 1].type) ||  IS_SIG_OPERATOR(tokens[i - 1].type) || tokens[i - 1].type == '(')) {
      tokens[i].type = TK_NEG;
    }
  } 
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  token_processing();

  word_t res = eval(0, nr_token - 1, success);

  return res;
}

void expr_test() {
  FILE *fp_in = fopen("tools/gen-expr/input.txt", "r");
  FILE *fp_out = fopen("output.txt", "w"); // 打开用于写入输出的文件
  assert(fp_in != NULL);
  assert(fp_out != NULL);

  char expression[65536];
  unsigned right_ans;
  for(int i = 0; i < 1000; ++i) {
    int t = fscanf(fp_in, "%u %s", &right_ans, expression);
    assert(t == 2);
    bool success = true;
    word_t res = expr(expression, &success);
    
    if(!success || (res != right_ans)) {
      fprintf(fp_out, "%d fali: flag: %d my_res:%u right_ans:%u expr:%s\n", i, success, res, right_ans, expression);
      //printf("no\n");
    }
    else  
      fprintf(fp_out, "%d ok\n", i);
  }

  fclose(fp_in);
  fclose(fp_out);
}