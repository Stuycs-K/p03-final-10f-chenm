#include "networking.h"


int server_setup(){
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  char* PORT = "9998";

  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, hints, &results);

  //Create socket
  int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  err(listen_socket, "socket error");

  int yes = 1;
  int sockOpt = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt, "sockopt error");

  int err = bind(listen_socket, results->ai_addr, results->ai_addrlen);
  if (err == -1){
    printf("Error binding: %s", strerror(errno));
    exit(1);
  }
  listen(listen_socket, 3);
  printf("Listening on port %s\n", PORT);

  


}
