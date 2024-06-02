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
  //printf("20\n");
  // // //sdb_mainloop();
  exec(); // ?
  //printf("30\n");
  finish_sim();
  
  return 0;
}

