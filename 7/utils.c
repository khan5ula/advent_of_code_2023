#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Hand* collect(FILE* file, int* count) {
  Hand* hands = malloc(INIT_S * sizeof(Hand));
  char line_of_text[LINE_S] = {'\0'};

  while (fgets(line_of_text, LINE_S, file)) {
    int index = 0;

    while (index < HAND_S) {
      hands[*count].cards[index] = line_of_text[index];
      index++;
    }

    hands[*count].bet = get_bet(line_of_text, index);
    (*count)++;

    if (*count % INIT_S == 0) {
      Hand* temp = realloc(hands, (*count * 2) * sizeof(Hand));
      if (!temp) {
        fprintf(stderr, "Error: Reallocation failed in function %s, line %d\n",
                __func__, __LINE__);
        free(hands);
        return NULL;
      } else {
        hands = temp;
      }
    }
  }

  return hands;
}

int get_bet(char* src, int index) {
  int bet = 0;
  char* end;

  char digits[BET_S] = {'\0'};

  for (int digitIndex = 0; index < strlen(src); index++, digitIndex++)
    digits[digitIndex] = src[index];

  bet = (int)strtol(digits, &end, 10);

  return bet;
}

long count_total(Hand* hands, int count) {
  long total = 0;

  get_strengths(hands, count);
  quicksort_hand(hands, 0, count - 1);
  total = count_from_sorted(hands, count);

  return total;
}

void get_strengths(Hand* hands, int count) {
  for (int hand = 0; hand < count; hand++) {
    int hits = 0;
    int result = 0;

    for (int card = 0; card < HAND_S; card++) {
      for (int other_c = 0; other_c < HAND_S; other_c++) {
        if (hands[hand].cards[card] == hands[hand].cards[other_c] &&
            card != other_c)
          hits++;
      }
    }

    switch (hits) {
      case 0:
        result = HIGH_CARD;
        break;
      case 2:
        result = ONE_PAIR;
        break;
      case 4:
        result = TWO_PAIR;
        break;
      case 6:
        result = THREE_OF_A_KIND;
        break;
      case 8:
        result = FULL_HOUSE;
        break;
      case 12:
        result = FOUR_OF_A_KIND;
        break;
      case 20:
        result = FIVE_OF_A_KIND;
        break;
      default:
        result = UNKNOWN;
        perror("Should have detected a proper weight");
        break;
    }

    hands[hand].strength = result;
  }
}

/**
 * Compares hand `h1` to hand `h2` based
 * on the strenght they hold.
 *
 * If the hands are identical,
 * compares individual cards starting
 * from left.
 * @return
 * `1` if h1 is larger.
 * `-1` if h1 is smaller.
 * `0` if h1 is equal to h2.
 */
int compare_hand(Hand h1, Hand h2) {
  int result = 0;

  if (h1.strength > h2.strength) {
    result = 1;
  } else if (h1.strength < h2.strength) {
    result = -1;
  } else {
    result = compare_single_cards(h1, h2);
  }
  return result;
}

int compare_single_cards(Hand h1, Hand h2) {
  int result = 0;

  for (int card = 0; card < HAND_S; card++) {
    int h1_comparable = h1.cards[card];
    int h2_comparable = h2.cards[card];

    if (!isdigit(h1.cards[card])) {
      h1_comparable = convert_card_to_digit(h1.cards[card]);
      h2_comparable = convert_card_to_digit(h2.cards[card]);
    }

    if (h1_comparable > h2_comparable) {
      result = 1;
      break;
    } else if (h1_comparable < h2_comparable) {
      result = -1;
      break;
    }
  }

  return result;
}

/**
 * Converts a `char` type card
 * to a numeric value for comparison.
 *
 * If one of the cards to be compared
 * is numeric, it should be converted
 * to `0`, since a `char` card is always
 * more valuable.
 */
int convert_card_to_digit(char card) {
  int result = 0;

  switch (card) {
    case 'A':
      result = 14;
      break;
    case 'K':
      result = 13;
      break;
    case 'Q':
      result = 12;
      break;
    case 'J':
      result = 11;
      break;
    case 'T':
      result = 10;
      break;
    default:
      result = 0;
      break;
  }

  return result;
}

void quicksort_hand(Hand* hands, int first_i, int last_i) {
  if (first_i < last_i) {
    int pivot = first_i;
    int smaller = first_i;
    int larger = last_i;

    while (smaller < larger) {
      while (compare_hand(hands[smaller], hands[pivot]) <= 0 &&
             smaller < last_i)
        smaller++;
      while (compare_hand(hands[larger], hands[pivot]) > 0)
        larger--;

      if (smaller < larger)
        swap(&hands[smaller], &hands[larger]);
    }

    swap(&hands[pivot], &hands[larger]);
    quicksort_hand(hands, first_i, larger - 1);
    quicksort_hand(hands, larger + 1, last_i);
  }
}

void swap(Hand* a, Hand* b) {
  Hand temp = *a;
  *a = *b;
  *b = temp;
}

long count_from_sorted(Hand* hands, int count) {
  long result = 0;

  for (int card = 0; card < count; card++)
    result += (card + 1) * hands[card].bet;

  return result;
}

void print_hands(Hand* hands, int count) {
  for (int hand = 0; hand < count; hand++) {
    printf("Hand:\t\t%s\n", hands[hand].cards);
    printf("Bet:\t\t%-d\n", hands[hand].bet);
    printf("Strength:\t%d\n", hands[hand].strength);
    printf("Rank:\t\t%d\n\n", hand + 1);
  }
}