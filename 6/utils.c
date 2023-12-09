#include "utils.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -------- part 1 -------- //

int get_digits(char* source, int array[], int length, char* searchBy) {
  char* srch;
  char* end;
  int count = 0;

  if ((srch = strstr(source, searchBy))) {
    srch = strtok(source, " ");

    while (srch) {
      if (isdigit(srch[0])) {
        array[count++] = (int)strtol(srch, &end, 10);
      }
      if (count > length)
        perror("exceeded the limit of time entries");

      srch = strtok(NULL, " ");
    }
  }

  return count;
}

int count_successful_ways(int times[], int distances[], int length) {
  int result = -1;

  for (int index = 0; index < length; index++) {
    int race_result = count_race_wins(times[index], distances[index]);
    result = (result == -1) ? race_result : result * race_result;
  }

  return result;
}

int count_race_wins(int time, int distance) {
  int result = 0;

  for (int ms = 0; ms <= time; ms++)
    if (((time - ms) * ms) > distance)
      result++;

  return result;
}

// -------- part 2 -------- //

long get_digit(char* src, char* searchBy) {
  long result = -1;

  if (!(strstr(src, searchBy))) {
    perror("couldn't read the source file");
    return result;
  }

  char digits[100] = {'\0'};
  int count = 0;
  char* end;

  for (int index = 0; index < strlen(src); index++)
    if (isdigit(src[index]))
      digits[count++] = src[index];

  result = strtol(digits, &end, 10);
  return result;
}

long count(long time, long distance) {
  long result = 0;

  for (long ms = 0; ms <= time; ms++)
    if (((time - ms) * ms) > distance)
      result++;

  return result;
}