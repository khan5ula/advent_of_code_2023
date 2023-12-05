#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int collect(const char* card,
            int collection[],
            const int size,
            const char start_from,
            const char end_by);

int main(void) {
  FILE* file;
  char card[1024] = {'\0'};
  int scratchcards_total = 0;
  int noOfLines = 0;
  int card_no = 0;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  while (fgets(card, 1024, file) != NULL)
    noOfLines++;

  rewind(file);

  int* copies = calloc((noOfLines + 1), sizeof(int));

  /**
   * DISCLAIMER:
   * The indexing on copies & card_no begins from 1,
   * meaning the array index of 0 is not used.
   */

  while (fgets(card, 1024, file) != NULL) {
    copies[++card_no]++;

    int winning_numbers[100] = {0};
    int playing_numbers[100] = {0};

    int winning_no_count = 0;
    int playing_no_count = 0;
    int cards_to_copy = 0;

    int multiplier = copies[card_no];

    winning_no_count = collect(card, winning_numbers, 100, ':', '|');
    playing_no_count = collect(card, playing_numbers, 100, '|', '\0');

    for (int pi = 0; pi < playing_no_count; pi++) {
      for (int wi = 0; wi < winning_no_count; wi++) {
        if (playing_numbers[pi] == winning_numbers[wi])
          cards_to_copy++;
      }
    }

    for (int index = card_no + 1; index <= card_no + cards_to_copy; index++)
      copies[index] += multiplier;
  }

  for (int index = 1; index <= noOfLines; index++)
    scratchcards_total += copies[index];

  printf("Scratchcards total: %d\n", scratchcards_total);

  free(copies);
  fclose(file);

  return 0;
}

int collect(const char* card,
            int collection[],
            const int size,
            const char start_from,
            const char end_by) {
  int index = 0;
  int consecutiveD = 0;
  int numbersFound = 0;
  int digits[2];

  while (card[index] != start_from)
    index++;

  while (index < strlen(card)) {
    if (isdigit(card[index])) {
      digits[consecutiveD++] = card[index] - '0';
    } else if (card[index] == end_by) {
      break;
    } else {
      if (consecutiveD == 1) {
        collection[numbersFound++] = digits[0];
      } else if (consecutiveD == 2) {
        collection[numbersFound++] = 10 * digits[0] + digits[1];
      }
      consecutiveD = 0;
    }
    index++;
  }

  return numbersFound;
}