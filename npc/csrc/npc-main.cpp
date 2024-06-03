

void init_sim();
void init_monitor(int argc, char *argv[]);
void sdb_mainloop();
void restart();
void finish_sim();
void exec();

int main(int argc, char **argv) {

  init_sim();

  init_monitor(argc, argv);

  restart();

  sdb_mainloop();
  
  finish_sim();

  return 0;
}

