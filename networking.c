#include "networking.h"
#include "blackjack.h"

#define PORT "9998"
#define MAX_CLIENTS 10
int player_done[MAX_CLIENTS];

int server_setup(){
  struct addrinfo hints, * results;
  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, PORT, &hints, &results) != 0){
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

  freeaddrinfo(results);

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

    int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
    if(activity < 0){
      perror("Select");
      continue;
    }

    if (FD_ISSET(listen_socket, &read_fds)){

      int client_socket = accept(listen_socket, NULL, NULL);
      printf("New player connected.\n");

      for (int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i] == -1){
          clients[i] = client_socket;
          player_done[i] = 0; //Player state, 0 = still playing, 1 = busted 

          write(client_socket, "Welcome to Blackjack\n", 22);
          
          //start round 
        if(!round_started){
          init_deck(deck);
          shuffle(deck);
          top = 0;
          dealer.count = 0;
          round_started = 1;

          deal_card(deck, &top, &dealer);
          deal_card(deck, &top, &dealer);
          }
          
          player_hands[i].count = 0;
          deal_card(deck, &top, &player_hands[i]);
          deal_card(deck, &top, &player_hands[i]);
          
          char msg[1024], buf[256];

        hand_to_string(&player_hands[i], buf);
        sprintf(msg, "\nYour hand: %s\n", buf);
        write(client_socket, msg, strlen(msg));

        //dealer's hand
        sprintf(msg, "Dealer's hand: [%d%c] [??]\n", dealer.cards[0].value, dealer.cards[0].suit);
        write(client_socket, msg, strlen(msg));
        write(client_socket, "Command: [hit/stand]:\n", 23);
        
        break;
        }
      }
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

          //Hit
          if (strncmp(buff, "hit", 3) == 0){
            if(player_done[i]){
              write(sd, "You are done this round.\n", 25);
              continue;
            }
            deal_card(deck, &top, &player_hands[i]);

            char msg[1024], buf[256];
            hand_to_string(&player_hands[i], buf);
            sprintf(msg, "Your hand: %s\n", buf);
            write(sd, msg, strlen(msg));

            if(hand_value(&player_hands[i]) > 21){
              write(sd, "Bust! You lose.\n", 16);
            }
          }

          //Stand
          else if (strncmp(buff, "stand", 5) == 0){
            if(player_done[i]){
              write(sd, "Already stood.\n", 16);
              continue;
            }

            player_done[i] = 1;
            write(sd, "You chose to stand.\n", 21);
            //dealer stops at 17
            while(hand_value(&dealer) < 17){
              deal_card(deck, &top, &dealer);
            }

            char msg[1024], buf[256];
            hand_to_string(&dealer, buf);
            sprintf(msg, "\nDealer's full hand: %s\n", buf);

            write(sd, msg, strlen(msg));

            int p = hand_value(&player_hands[i]);
            int d = hand_value(&dealer);

            if(d > 21 || p > d) write(sd, "You win!\n", 9);
            else if(p < d) write(sd, "Dealer wins.\n", 14);
            else write(sd, "Push (tie).\n", 12);
          } else{
            write(sd, "Invalid! Chose hit or stand...or just stop gambling.\n", 54);
          }
        }
      }
    }
  }

  return listen_socket;
}
