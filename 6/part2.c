#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "running_time_counter.h"
#include "utils.h"

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

  rtimer_init();

  char line_of_text[1024] = {'\0'};
  fgets(line_of_text, 1024, file);
  long time = get_digit(line_of_text, "Time");
  fgets(line_of_text, 1024, file);
  long distance = get_digit(line_of_text, "Distance");

  fclose(file);

  printf("\n**************************************\n");
  printf("time:\t\t%10ld\n", time);
  printf("distance:\t%10ld\n", distance);

  long result = count(time, distance);

  printf("result:\t\t%10ld\n\n", result);

  rtimer_end();
  rtimer_results();
  printf("**************************************\n");

  return 0;
}