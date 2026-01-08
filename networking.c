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

  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);

  fd_set read_fds;
  char buff[1025] ="";

  while(1){

    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(listen_socket, &read_fds);
    int i = select(listen_socket+1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(listen_socket, &read_fds)){

      int client_socket = accept(listen_socket, (struct sockaddr *)&client_address, &sock_size);
      printf("Connected, waiting for data.\n");

      read(client_socket, buff, sizeof(buff));

      buff[strlen(buff) -1] = 0; //clear newline
      if(buff[strlen(buff) - 1 ] == 13){
        //clear windows line ending
        buff[strlen(buff) -1] = 0;

      }
      printf("\nReceieved from client '%s'\n", buff);
      close(client_socket);
  }

  free(hints);
  freeaddrinfo(results);
  return 0;


}
