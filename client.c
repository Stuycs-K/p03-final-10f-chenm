#include "networking.h"

//non blocking reads
void flush_serv_msg(int sockfd){
    char buf[BUFFER_SIZE];
    int n;

    while(1){
        fd_set fds;
        struct timeval tv;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;

        int ret = select(sockfd + 1, &fds, NULL, NULL, &tv);
        if(ret <= 0) break;

        n = read(sockfd, buf, sizeof(buf)-1);
        if(n<=0){
            printf("\nServer diconnected.\n");
            close(sockfd);
            exit(0);
        } 
        buf[n] = 0;
        printf("%s", buf);
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
    flush_serv_msg(sockfd);

    char buffer[BUFFER_SIZE];
    while(1){
        
        printf("\nCommand [hit/stand]: ");
        fflush(stdout);
        
        if(!fgets(buffer, sizeof(buffer), stdin)) break;

        buffer[strcspn(buffer, "\n")] = 0;

        write(sockfd, buffer, strlen(buffer));
        flush_serv_msg(sockfd);

    }

    printf("Server disconnected.\n");
    close(sockfd);
    return 0;
}
