#include "networking.h"
#include "blackjack.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int listen_socket;
struct card deck[52];
int top = 0;
struct hand dealer;
int round_started = 0;


static void signal_Handler(int sig){
  if(sig == SIGINT){
    printf("\nServer shutting down...\n");
    if(listen_socket > 0) close(listen_socket);
    exit(0);
  }

}

int main(){
  
  signal(SIGINT, signal_Handler);
  
  //server starts
  listen_socket = server_setup();

  return 0;

}
