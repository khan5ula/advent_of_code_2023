#define _POSIX_C_SOURCE 199309L  // https://stackoverflow.com/a/8881675/21125767

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "seeds.h"

#define INITIAL_SIZE 10

int main(int argc, char* argv[]) {
  if (argc < 2) {
    perror("expected source as launch parameter");
    return 1;
  }

  FILE* file;
  if ((file = fopen(argv[1], "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  struct timespec starting_time, ending_time;
  clock_gettime(CLOCK_MONOTONIC, &starting_time);

  unsigned long lowest_location = -1;
  struct seeds_t seeds = {malloc(INITIAL_SIZE * sizeof(unsigned long)), 0};
  struct targets_t targets = init_targets();
  char line_of_text[1024] = {'\0'};

  while (fgets(line_of_text, 1024, file)) {
    collect_seeds(&seeds, file, line_of_text);
    collect_targets(&targets, file, line_of_text);
  }

  fclose(file);

  // lowest_location = go_through_from_seeds(&seeds, targets);
  lowest_location = go_through_from_location(&seeds, targets);  // faster

  clock_gettime(CLOCK_MONOTONIC, &ending_time);

  long elapsed_time_ns =
      (ending_time.tv_sec - starting_time.tv_sec) * 1000000000 +
      (ending_time.tv_nsec - starting_time.tv_nsec);

  print_results(lowest_location, seeds, elapsed_time_ns);
  free(seeds.list);
  free_targets(&targets);

  return 0;
}