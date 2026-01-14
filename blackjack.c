#include "blackjack.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

void init_deck(struct card *deck){
    char suits[] = {'H', 'D', 'C', 'S'};
    int index = 0;

    for (int s = 0; s<4; s++){
        for (int v = 1; v <= 13; v++){
            deck[index].suit = suits[s];
            deck[index].value = (v>10) ? 10 : v;
            index++;
        }
    }
}

void shuffle(struct card *deck){
    srand(time(NULL));
    for (int i = 0; i < 52; i++){
        int j = rand() % 52;
        struct card tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }
}

void deal_card(struct card *deck, int *top, struct hand *h){
    h->cards[h->count++] = deck[(*top)++];
}

void hand_to_string(struct hand *h, char *buffer){
    buffer[0] = 0;
    char temp[32];

    for(int i = 0; i < h->count; i++){
        sprintf(temp, "[%d%c]", h->cards[i].value, h->cards[i].suit);

        strcat(buffer, temp);
    }
}

//Adds up all card values, all jack, queens, and kings are 10, and aces(1 or 11)
int hand_value(struct hand *h){
  int sum = 0;
  int aces = 0;

  for(int i = 0; i < h->count; i++){
    int v = h->cards[i].value;

    if(v == 1){
        aces++;
        sum += 11;
    } else{
        sum += v;
    }
  }

  //Ace value(1 or 11)
  while(sum > 21 && aces > 0){
    sum -= 10;
    aces--;
  }

  return sum;

}
