#include "networking.h"
#include "blackjack.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//global vars
struct card deck[MAX_DECK];
int top = 0;
struct hand dealer;
int round_started = 0;

int listen_socket;

static void signal_Handler(int sig){
  if(sig == SIGINT){
    printf("\nServer shutting down...\n");
    if(listen_socket > 0) close(listen_socket);
    exit(0);
  }
}

int main(){

  signal(SIGINT, signal_Handler);

  
  listen_socket = server_select();

  return 0;
}
