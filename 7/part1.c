#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    perror("expected source as launch parameter");
    return 1;
  }

  FILE* file;
  if (!(file = fopen(argv[1], "r"))) {
    perror("couldn't open the file");
    return 1;
  }

  int count_of_hands = 0;
  Hand* hands = collect(file, &count_of_hands);
  long total = count_total(hands, count_of_hands);

  printf("Total winnings: %ld\n\n", total);
  print_hands(hands, count_of_hands);

  fclose(file);
  free(hands);
  return 0;
}