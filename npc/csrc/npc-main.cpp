

int main(int argc, char **argv) {
  
  init_sim();

  init_monitor(argc, argv);

  restart();

  sdb_mainloop();

  finish_sim();
  
  return 0;
}

