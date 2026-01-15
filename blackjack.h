#ifndef BLACKJACK_H
#define BLACKJACK_H

#define MAX_DECK 52
#define MAX_HAND 12

struct card{
    int value; // 1-11
    char suit; //H: Heart, D: Diamond, C: Clubs, S: SPADE
};

struct hand {
    struct card cards[MAX_HAND];
    int count;
};

void init_deck(struct card *deck); //Starting deck
void shuffle(struct card *deck); //Randomize deck
void deal_card(struct card *deck, int *top, struct hand *h); //Keeping track of which cards have been dealt with
int hand_value(struct hand *h);//Calculating the value of each hand

void hand_to_string(struct hand *h, char *buffer); //Printing cards

//Global values
extern struct card deck[52];
extern int top;
extern struct hand dealer;
extern int round_started;
extern struct hand player_hands[MAX_CLIENTS];

#endif
