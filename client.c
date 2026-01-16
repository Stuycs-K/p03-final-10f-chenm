#include "networking.h"


void flush_serv_msg(int sockfd){
    char buf[BUFFER_SIZE];
    int n;

    while((n = read(sockfd, buf, sizeof(buf) - 1)) > 0){
        buf[n] = 0;
        printf("%s", buf);

        if(n<sizeof(buf) - 1) break;
    }
}

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

    char buffer[BUFFER_SIZE];
    while(1){
        flush_serv_msg(sockfd);
        
        printf("\nCommand [hit/stand]: ");
        fflush(stdout);
        
        if(!fgets(buffer, sizeof(buffer), stdin)) break;

        buffer[strcspn(buffer, "\n")] = 0;

        if(strlen(buffer) == 0) continue;
        
        write(sockfd, buffer, strlen(buffer));
        flush_serv_msg(sockfd);

    }

    printf("Server disconnected.\n");
    close(sockfd);
    return 0;
}
