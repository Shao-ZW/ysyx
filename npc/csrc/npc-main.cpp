#include <cstdio>

void init_sim();
void restart();
void finish_sim();
void exec();

int main(int argc, char **argv) {
  //printf("10\n");
  init_sim();

  //init_monitor(argc, argv);

  restart();

  // // //sdb_mainloop();
  exec(); // ?

  finish_sim();
  
  return 0;
}

