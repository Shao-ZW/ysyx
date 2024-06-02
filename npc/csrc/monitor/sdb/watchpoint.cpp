#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  int old_val;
  char expression[128];
} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
static int cnt = 0;

void init_wp_pool() {
  for (int i = 0; i < NR_WP; i ++) 
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  
  head = NULL;
  free_ = wp_pool;
}

static WP* new_wp() {
  WP *p = free_;
  assert(p != NULL);
  free_ = p->next;
  p->next = head;
  head = p;
  
  return p;
}

void free_wp(int n) {
  WP *p, *lp;
  
  for(p = lp = head; p; p = p->next) {
    if(p->NO == n) {
      if(p == head) 
        head = p->next;
      else 
        lp->next = p->next;
      p->next = free_;
      free_ = p;
      return;
    }

    lp = p;
  }
}

void wp_display() {
  printf("Num    Disp    Enb    What\n");

  if(head == NULL)
    printf("NONE   NONE    NONE   NONE\n");

  for(WP *p = head; p; p = p->next) 
    printf("%-3d    keep    y      %s\n", p->NO, p->expression);
}

void add_wp(char *expression, word_t val) {
  WP *p = new_wp();
  p->old_val = val;
  p->NO = cnt++;
  strncpy(p->expression, expression, 127);
  p->expression[127] = '\0';
}

void check_wp() {
  bool flag = false;

  for(WP *p = head; p; p = p->next) {
    bool success = true;
    word_t new_val = expr(p->expression, &success);
    assert(success);
    if(p->old_val != new_val) {
      printf("%-3d %-10s old val: %-5u new val: %-5u\n", p->NO, p->expression, p->old_val, new_val);
      p->old_val = new_val;
      flag = true;
    }
  }

  if(flag) 
    nemu_state.state = NEMU_STOP;
}