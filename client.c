#include "networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER_PORT "9998"

int main(int argc, char *argv[]) {
    char* IP = "127.0.0.1";
    if(argc > 1){
      IP=argv[1];
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(IP, SERVER_PORT, &hints, &res) != 0){
        perror("getaddrinfo");
        exit(1);
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd < 0){
        perror("socket");
        exit(1);
    }

    if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0){
        perror("connect");
        exit(1);
    }

    freeaddrinfo(res);

    printf("Connected to server!\n");

    char buff[1024];
    int n;

    // Receive messages from server
    while((n = read(sockfd, buff, sizeof(buff)-1)) > 0){
        buff[n] = 0;
        printf("%s", buff);
    }

    char buffer[BUFFER_SIZE];

    while(1){
      printf("Command: ");
      fflush(stdout);

      if(!fgets(buffer, BUFFER_SIZE, stdin)){
        printf("Waiting for command: [hit] or [stand]\n");
        break;
      }

      write(server_socket, buffer, strlen(buffer) + 1);

      int bytes = read(sockfd, buffer, BUFFER_SIZE);
      if(bytes <= 0) break;

    }
    printf("Client received: %s\n");

    printf("Server disconnected.\n");
    close(sockfd);
    return 0;
}
