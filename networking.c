#include "networking.h"
#include "blackjack.h'

#define PORT "9998"
#define MAX_CLIENTS 10

int server_setup(){
  struct addrinfo * hints, * results;
  memset(&hints, 0, sizeof(hints));

  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, PORT, hints, &results) != 0){
    perror("getaddrinfo");
    exit(1);
  }

  //Create socket
  int listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (listen_socket == -1){
    perror("socket");
    exit(1);
  }

  int yes = 1;
  setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  if (bind(listen_socket, results->ai_addr, results->ai_addrlen) == -1){
    perror("bind");
    exit(1);
  }

  listen(listen_socket, 10);
  printf("Server listening on port %s\n", PORT);

  int clients[MAX_CLIENTS];
  for(int i = 0; i < MAX_CLIENTS; i++) clients[i] = -1;


  fd_set read_fds;

  while(1){

    FD_ZERO(&read_fds);
    FD_SET(listen_socket, &read_fds);

    int max_fd = listen_socket;

    for (int i = 0; i < MAX_CLIENTS; i++){
      if (clients[i] != -1){
        FD_SET(clients[i], &read_fds);
        if (clients[i] > max_fd) max_fd = clients[i];
      }
    }

    select(max_fd + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(listen_socket, &read_fds)){

      int client_socket = accept(listen_socket, NULL, NULL);
      printf("New player connected.\n");

      for (int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i] == -1){
          clients[i] = client_socket;
          write(client_socket, "Welcome to Blackjack\n", 23);
          break;
        }
      }
      if(!round_started){
        init_deck(deck);
        shuffle(deck);
        top = 0;
        dealer.count = 0;
        round_started = 1;
      }

      struct hand player;
      player.count = 0;

      deal_card(deck, &top, &player);
      deal_card(deck, &top, &player);

      if(dealer.count < 2){
        deal_card(deck, &top, &dealer);
        deal_card(deck, &top, &dealer);
      }
      char msg[1024];
      char handbuf[256];

      hand_to_string(&player, handbuf);
      sprintf(msg, "Your hand: %s\n", handbuf);
      write(client_socket, msg, strlen(msg));

      hand_to_string(&dealer, handbuf);
      sprintf(msg, "Dealer's hand: %s\n", handbuf);
      write(client_socket, msg, strlen(msg));
    }

    for(int i = 0; i < MAX_CLIENTS; i++){
      int sd = clients[i];
      if (sd != -1 && FD_ISSET(sd, &read_fds)){
        char buff[1024];
        int bytes = read(sd, buff, sizeof(buff) - 1);

        if (bytes <= 0){
          printf("Player disconnected.\n");
          close(sd);
          clients[i] = -1;
        } else {
          buff[bytes] = 0;
          printf("Player says: %s", buff);
        }
      }
    }
  }

  return listen_socket;
}
