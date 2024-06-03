// #include <isa.h>
// #include <cpu/cpu.h>
// #include <readline/readline.h>
// #include <readline/history.h>
// #include "sdb.h"

static int is_batch_mode = false;

void init_regex();
// void init_wp_pool();
// void add_wp(char*, word_t);
// void free_wp(int);
// void wp_display();
// word_t expr(char *e, bool *success);

// /* We use the `readline' library to provide more flexibility to read from stdin. */
// static char* rl_gets() {
//   static char *line_read = NULL;

//   if (line_read) {
//     free(line_read);
//     line_read = NULL;
//   }

//   line_read = readline("(nemu) ");

//   if (line_read && *line_read) {
//     add_history(line_read);
//   }

//   return line_read;
// }

// static int cmd_c(char *args) {
//   cpu_exec(-1);
//   return 0;
// }


// static int cmd_q(char *args) {
//   npc_state.state = NEMU_QUIT;
//   return -1;
// }

// static int cmd_help(char *args);
// static int cmd_si(char *args);
// static int cmd_info(char *args);
// static int cmd_x(char *args);
// static int cmd_p(char *args);
// static int cmd_w(char *args);
// static int cmd_d(char *args);

// static struct {
//   const char *name;
//   const char *description;
//   int (*handler) (char *);
// } cmd_table [] = {
//   { "help", "Display information about all supported commands", cmd_help },
//   { "c", "Continue the execution of the program", cmd_c },
//   { "q", "Exit NEMU", cmd_q },
//   { "si", "Execute n(default n=1) instructions in single steps of the program", cmd_si },
//   { "info", "Get infomation about the program", cmd_info },
//   { "x", "Scan the memory", cmd_x },
//   { "p", "Evaluate an expression", cmd_p },
//   { "w", "Set watchpoint", cmd_w },
//   { "d", "Delete watchpoint", cmd_d }
// };

// #define NR_CMD ARRLEN(cmd_table)

// static int cmd_help(char *args) {
//   /* extract the first argument */
//   char *arg = strtok(NULL, " ");
//   int i;

//   if (arg == NULL) {
//     /* no argument given */
//     for (i = 0; i < NR_CMD; i ++) {
//       printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
//     }
//   }
//   else {
//     for (i = 0; i < NR_CMD; i ++) {
//       if (strcmp(arg, cmd_table[i].name) == 0) {
//         printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
//         return 0;
//       }
//     }
//     printf("Unknown command '%s'\n", arg);
//   }
//   return 0;
// }

// static int cmd_si(char *args) {
//   if(args == NULL)
//     cpu_exec(1);
//   else {
//     int step;
//     int t = sscanf(args, "%d", &step);

//     if(t == 1)
//       cpu_exec(step);
//     else 
//       printf("Usage: si [N]\n");
//   }
//   return 0;
// }

// static int cmd_info(char *args) {
//   if(args == NULL) {
//     printf("Usage: info SUBCMD\n");
//     return 0;
//   }

//   char type = 0;
//   sscanf(args, "%c", &type);

//   if(type == 'r') {
//     isa_reg_display();
//   } else if(type == 'w') {
//     wp_display();
//   } else {
//     printf("Unknown argument %s\n", args);
//   } 
//   return 0;
// }

// static int cmd_x(char *args) {
//   // if(args == NULL) {
//   //   printf("Usage: x N EXPR\n");
//   //   return 0;
//   // }

//   // int n;
//   // char expression[128];
//   // int t = sscanf(args, "%d %[^\n]", &n, expression);

//   // if(t == 2) {
//   //   bool success = true;
//   //   vaddr_t addr = expr(expression, &success);

//   //   if(success) {
//   //     for(int i = 0; i < n; i++) {
//   //       word_t val = vaddr_read(addr, 4);
//   //       printf("0x%08x: %08x\n", addr, val);
//   //       addr += 4;
//   //     }
//   //   }
//   //   else 
//   //     printf("x: please give a proper expression\n");
//   // }
//   // else 
//   //   printf("Usage: x N EXPR\n");
  
//   // return 0;
// }

// static int cmd_p(char *args) {
//   char *expression = args;
//   bool success = true;
//   word_t ans = expr(expression, &success);

//   if(success)
//     printf("%s = %u\n", expression, ans);
//   else
//     printf("p: please give a proper expression\n");

//   return 0;
// }

// static int cmd_w(char *args) {
//   char *expression = args;
//   bool success = true;
//   word_t val = expr(expression, &success);

//   if(success)
//     add_wp(expression, val);
//   else 
//     printf("w: please give a proper expression\n");

//   return 0;
// }

// static int cmd_d(char *args) {
//   int n;
//   int t = sscanf(args, "%d", &n);

//   if(t == 1) 
//     free_wp(n);
//   else 
//     printf("Usage: d N\n");

//   return 0;
// }

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

// void sdb_mainloop() {
//   if (is_batch_mode) {
//     cmd_c(NULL);
//     return;
//   }

//   for (char *str; (str = rl_gets()) != NULL; ) {
//     char *str_end = str + strlen(str);

//     /* extract the first token as the command */
//     char *cmd = strtok(str, " ");
//     if (cmd == NULL) { continue; }

//     /* treat the remaining string as the arguments,
//      * which may need further parsing
//      */
//     char *args = cmd + strlen(cmd) + 1;
//     if (args >= str_end) {
//       args = NULL;
//     }

// #ifdef CONFIG_DEVICE
//     extern void sdl_clear_event_queue();
//     sdl_clear_event_queue();
// #endif

//     int i;
//     for (i = 0; i < NR_CMD; i ++) {
//       if (strcmp(cmd, cmd_table[i].name) == 0) {
//         if (cmd_table[i].handler(args) < 0) { return; }
//         break;
//       }
//     }

//     if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
//   }
// }

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  //init_wp_pool();
}