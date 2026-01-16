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
struct hand player_hands[10];
extern void shutdown_server();

int listen_socket;

void shutdown_server(); 
static void signal_Handler(int sig){
  if(sig == SIGINT || sig == SIGTERM){
    printf("\nServer shutting down...\n");
    shutdown_server();
    if(listen_socket > 0) close(listen_socket);
    exit(0);
  }
}

int main(){

  signal(SIGINT, signal_Handler);
  signal(SIGTERM, signal_Handler);

  listen_socket = server_setup();

  return 0;
}
