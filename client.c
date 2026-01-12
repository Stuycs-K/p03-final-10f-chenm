#include "networking.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#define SERVER_PORT "9998"
#define SERVER_HOST "localhost"

int main() {

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if(getaddrinfo(SERVER_HOST, SERVER_PORT, &hints, &res) != 0){
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

    printf("Server disconnected.\n");
    close(sockfd);
    return 0;
}
