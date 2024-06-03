#include <cstdio>

void init_sim();
void init_monitor(int argc, char *argv[]);
void restart();
void finish_sim();
void exec();

int main(int argc, char **argv) {

  init_sim();

  init_monitor(argc, argv);

  restart();

  //sdb_mainloop();
  for(int i = 0; i < 20; ++i)
    exec(); // ?

  finish_sim();

  return 0;
}

