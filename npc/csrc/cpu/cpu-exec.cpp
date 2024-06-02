// // #include <cpu/cpu.h>
// // #include <cpu/difftest.h>
// // #include <locale.h>
// #include <cstdint>
// #include <utils.h>
// /* The assembly code of instructions executed is only output to the screen
//  * when the number of instructions executed is less than this value.
//  * This is useful when you use the `si' command.
//  * You can modify this value as you want.
//  */
// #define MAX_INST_TO_PRINT 10

// uint64_t g_nr_guest_inst = 0;
// static uint64_t g_timer = 0; // unit: us
// static bool g_print_step = false;
// NPCState npc_state = { .state = NPC_STOP };

// void device_update();
// void check_wp();
// void iringbuffer_display();
// void ftrace_display();

// // static void trace_and_difftest() {
// // #ifdef CONFIG_ITRACE_COND
// //   if (ITRACE_COND) { log_write("%s\n", _this->logbuf); }
// // #endif
// //   if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
// //   IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));

// //   IFDEF(CONFIG_WATCHPOINT, check_wp());
// // }

// static void exec_once() {
//   eval(1);
//   eval(0);
// }

// static void execute(uint64_t n) {
//   for (;n > 0; n --) {
//     exec_once();
//     g_nr_guest_inst ++;
//     //trace_and_difftest();
//     //if (npc_state.state != NPC_RUNNING) break;
//     //IFDEF(CONFIG_DEVICE, device_update());
//   }
// }

// static void statistic() {
// //   IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
// // #define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
// //   Log("host time spent = " NUMBERIC_FMT " us", g_timer);
// //   Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
// //   if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
// //   else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
// }

// void assert_fail_msg() {
//   //reg_display();
//   // IFDEF(CONFIG_ITRACE, iringbuffer_display());
//   // IFDEF(CONFIG_FTRACE, ftrace_display());
//   statistic();
// }

// void cpu_exec(uint64_t n) {
//   g_print_step = (n < MAX_INST_TO_PRINT);
//   switch (npc_state.state) {
//     case NPC_END: case NPC_ABORT:
//       printf("Program execution has ended. To restart the program, exit NPC and run again.\n");
//       return;
//     default: npc_state.state = NPC_RUNNING;
//   }

//   //uint64_t timer_start = get_time();

//   execute(n);

//   //uint64_t timer_end = get_time();
//   //g_timer += timer_end - timer_start;

//   switch (npc_state.state) {
//     case NPC_RUNNING: npc_state.state = NPC_STOP; break;

//     case NPC_END: case NPC_ABORT:
//       Log("nemu: %s at pc = " FMT_WORD,
//           (npc_state.state == NPC_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
//            (npc_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
//             ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
//           npc_state.halt_pc);
//       // fall through
//     case NPC_QUIT: statistic();
//   }
// }
