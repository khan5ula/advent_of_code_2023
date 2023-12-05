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
  int sum = 0;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  while (fgets(card, 1024, file) != NULL || !feof(file)) {
    int winning_numbers[100] = {0};
    int playing_numbers[100] = {0};
    int winning_no_count = 0;
    int playing_no_count = 0;
    int matches = 0;

    // collect winning numbers
    winning_no_count = collect(card, winning_numbers, 100, ':', '|');

    // collect playing numbers
    playing_no_count = collect(card, playing_numbers, 100, '|', '\0');

    for (int pi = 0; pi < playing_no_count; pi++) {
      for (int wi = 0; wi < winning_no_count; wi++) {
        if (playing_numbers[pi] == winning_numbers[wi])
          matches = (matches == 0) ? 1 : matches * 2;
      }
    }

    sum += matches;
  }

  printf("The total sum is: %d\n", sum);
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
      if (consecutiveD + 1 >= size) {
        perror("exceeded limit of the array");
        return -1;
      }
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