#include "networking.h"

static void signal_Handler(int sig){
  if(sig == SIGINT){
    printf("Receieved signal: %d\n", sig);
    exit(0);
  }

}

int main(){

  signal(SIGINT, signal_Handler);

  pid_t p = getpid();
  kill(p, SIGINT);

  return 0;
}
