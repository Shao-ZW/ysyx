#include "common.h"

void init_monitor(int argc, char *argv[]);
void init_sim();
void sdb_mainloop();
void finish_sim();

int main(int argc, char **argv) {

  init_monitor(argc, argv);

  init_sim();

  sdb_mainloop();

  finish_sim();

  return 0;
}
