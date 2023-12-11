#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define INIT_S 20      // Initial size of a collection of hands
#define LINE_S 12      // Maximum characters per a line in source file
#define BET_S 6        // Max number of chars in a bet incl. null terminator
#define HAND_S 5       // Cards in a hand
#define STRENGHTS_C 8  // Number of different strenghts

// Strengths of hands
#define FIVE_OF_A_KIND 7
#define FOUR_OF_A_KIND 6
#define FULL_HOUSE 5
#define THREE_OF_A_KIND 4
#define TWO_PAIR 3
#define ONE_PAIR 2
#define HIGH_CARD 1
#define UNKNOWN 0

typedef struct Hand {
  char cards[HAND_S + 1];
  int bet;
  int strength;
} Hand;

Hand* collect(FILE* file, int* count);
int get_bet(char* src, int index);
long count_total(Hand* hands, int count);
void get_strengths(Hand* hands, int count);
int compare_hand(Hand h1, Hand h2);
int compare_single_cards(Hand h1, Hand h2);
int convert_card_to_digit(char card);
void quicksort_hand(Hand* hands, int first_i, int last_i);
void swap(Hand* a, Hand* b);
long count_from_sorted(Hand* hands, int count);
void print_hands(Hand* hands, int count);

#endif