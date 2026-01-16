#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER_PORT "9998"
#define BUFFER_SIZE 1024

int server_setup(void);
void shutdown_server(void);
void broadcast(int clients[], char *msg);
int all_players_done(int clients[], int player_done[]);

#endif
