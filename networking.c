#include "networking.h"
#include "blackjack.h"

#define PORT "9998"
#define MAX_CLIENTS 10
int clients[MAX_CLIENTS];
int player_done[MAX_CLIENTS];

//HElper to check if all players are done
int all_players_done(int clients[], int player_done[]){
  for(int i = 0; i < MAX_CLIENTS; i++){
    if(clients[i] != -1 && player_done[i] == 0){
      return 0;
    }
  }
  return 1;
}

void broadcast(int clients[], char *msg){
  for(int i = 0; i < MAX_CLIENTS; i++){
    if(clients[i] != -1){
      write(clients[i], msg, strlen(msg));
    }
  }
}

void shutdown_server(){
  char *msg = "\n Server is shutting down. Goodbye. \n";

  for(int i = 0; i < MAX_CLIENTS; i++){
    if(clients[i] != -1){
      write(clients[i], msg,strlen(msg));
      close(clients[i]);
      clients[i] = -1;
    }
  }
}

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

  listen_socket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if(listen_socket == -1){
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

  for(int i = 0; i < MAX_CLIENTS; i++){
    clients[i] = -1;
    player_done[i] = 0;
  }

  fd_set read_fds;

  while(1){

    FD_ZERO(&read_fds);
    FD_SET(listen_socket, &read_fds);

    int max_fd = listen_socket;
    for(int i = 0; i < MAX_CLIENTS; i++){
      if(clients[i] != -1){
        FD_SET(clients[i], &read_fds);
        if(clients[i] > max_fd) max_fd = clients[i];
      }
    }

    if(select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0){
      perror("select");
      continue;
    }



    if(FD_ISSET(listen_socket, &read_fds)){

      int client_socket = accept(listen_socket, NULL, NULL);

      printf("New player connected.\n");

      for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i] == -1){

          clients[i] = client_socket;
          player_done[i] = 0;

          write(client_socket, "Welcome to Blackjack\n", 22);

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

          sprintf(msg, "Dealer's hand: [%d%c] [??]\n", dealer.cards[0].value, dealer.cards[0].suit);
          write(client_socket, msg, strlen(msg));
          write(client_socket, "Command [hit/stand]: ", 22);

          break;
        }
      }
    }

    for(int i = 0; i < MAX_CLIENTS; i++){

      int sd = clients[i];
      if(sd == -1) continue;

      if(FD_ISSET(sd, &read_fds)){

        char buff[1024];
        int bytes = read(sd, buff, sizeof(buff)-1);

        if(bytes <= 0){
          printf("Player disconnected.\n");
          close(sd);
          clients[i] = -1;
          continue;
        }

        buff[bytes] = 0;
        if(buff[bytes-1] == '\n') buff[bytes-1] = 0;

        //hit
        if(strncmp(buff, "hit", 3) == 0){

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
            player_done[i] = 1;
          }
          
          sprintf(msg, "Dealer's hand: [%d%c] [??]\n", dealer.cards[0].value, dealer.cards[0].suit);
          write(sd, msg,strlen(msg));

          write(sd, "Command [hit/stand]: ", 22);
        }

        //stand
        else if(strncmp(buff, "stand", 5) == 0){

          if(player_done[i]){
            write(sd, "Already stood.\n", 16);
            write(sd, "Command [hit/stand]: ", 22);
            continue;
          }

          player_done[i] = 1;
          write(sd, "You chose to stand.\n", 21);

          if(all_players_done(clients, player_done)){

            while(hand_value(&dealer) < 17){
              deal_card(deck, &top, &dealer);
            }

            char msg[1024], buf[256];
            hand_to_string(&dealer, buf);
            sprintf(msg, "\n=== Dealer's full hand: %s ===\n", buf);
            broadcast(clients, msg);

            int d = hand_value(&dealer);

            for(int j = 0; j < MAX_CLIENTS; j++){
              if(clients[j] != -1){

                int p = hand_value(&player_hands[j]);

                if(p > 21) write(clients[j], "You busted!\n", 12);
                else if(d > 21 || p > d) write(clients[j], "You win!\n", 9);
                else if(p < d) write(clients[j], "Dealer wins.\n", 14);
                else write(clients[j], "Push (tie).\n", 12);

                write(clients[j], "Command [hit/stand]: ", 22);
              }
            }
            //resets round
            round_started = 0;
            dealer.count = 0;
            top = 0;
            for(int j = 0; j < MAX_CLIENTS; j++){
              player_done[j] = 0;
              player_hands[j].count = 0;
            }
          }
        }


        else{
          write(sd, "Invalid! Choose hit or stand...or quit\n", 40);
          write(sd, "Command [hit/stand]: ", 22);
        }
      }
    }
  }
  return listen_socket;
}