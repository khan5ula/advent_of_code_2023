#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

#define ARRAY_LENGTH 4

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

  int times[ARRAY_LENGTH] = {0};
  int distances[ARRAY_LENGTH] = {0};

  char line_of_text[1024] = {'\0'};

  fgets(line_of_text, 1024, file);
  int count_of_games = get_digits(line_of_text, times, ARRAY_LENGTH, "Time");
  fgets(line_of_text, 1024, file);
  get_digits(line_of_text, distances, ARRAY_LENGTH, "Distance");

  fclose(file);

  int successful_ways_multiplied =
      count_successful_ways(times, distances, count_of_games);

  clock_gettime(CLOCK_MONOTONIC, &ending_time);

  long elapsed_time_ns =
      (ending_time.tv_sec - starting_time.tv_sec) * 1000000000 +
      (ending_time.tv_nsec - starting_time.tv_nsec);

  printf("successful ways: %d\n", successful_ways_multiplied);
  printf("Running time: %.3lf ms, %.2lf seconds\n",
         (double)elapsed_time_ns / 1000000,
         (double)elapsed_time_ns / 1000000000);

  return 0;
}