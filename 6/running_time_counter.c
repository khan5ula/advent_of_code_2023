#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct timespec s, e;

void rtimer_init() {
  clock_gettime(CLOCK_MONOTONIC, &s);
}

void rtimer_end() {
  clock_gettime(CLOCK_MONOTONIC, &e);
}

void rtimer_results() {
  long ns = (e.tv_sec - s.tv_sec) * 1000000000 + (e.tv_nsec - s.tv_nsec);

  printf("Running time: %.3lf ms, %.2lf seconds\n", (double)ns / 1000000,
         (double)ns / 1000000000);
}