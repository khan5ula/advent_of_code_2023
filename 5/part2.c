#include "seeds.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_SIZE 10

typedef struct seeds_t {
  unsigned long* list;
  int no_of_refs;
  unsigned long total_count;
} seeds_t;

typedef struct targets_t {
  unsigned long* soils;
  unsigned long* fertilizers;
  unsigned long* water;
  unsigned long* light;
  unsigned long* temperature;
  unsigned long* humidity;
  unsigned long* location;

  int no_of_soils;
  int no_of_fertilizers;
  int no_of_water;
  int no_of_light;
  int no_of_temp;
  int no_of_humidity;
  int no_of_location;
} targets_t;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    perror("expected source as launch parameter");
    return 1;
  }

  FILE* file;

  struct timespec starting_time, ending_time;
  clock_gettime(CLOCK_MONOTONIC, &starting_time);

  if ((file = fopen(argv[1], "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  char line_of_text[1024] = {'\0'};

  struct seeds_t seeds = {malloc(INITIAL_SIZE * sizeof(unsigned long)), 0, 0};

  struct targets_t targets = {
      .soils = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .fertilizers = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .water = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .light = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .temperature = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .humidity = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .location = malloc(INITIAL_SIZE * sizeof(unsigned long)),
      .no_of_soils = 0,
      .no_of_fertilizers = 0,
      .no_of_water = 0,
      .no_of_light = 0,
      .no_of_temp = 0,
      .no_of_humidity = 0,
      .no_of_location = 0};

  unsigned long lowest_location = -1;

  while (fgets(line_of_text, 1024, file)) {
    char* ch;

    if ((ch = strstr(line_of_text, "seeds:")))
      seeds.no_of_refs = collect_seeds(&seeds.list, seeds.no_of_refs, ch);

    if ((ch = strstr(line_of_text, "seed-to-soil map:")))
      targets.no_of_soils = collect_types(&targets.soils, targets.no_of_soils,
                                          line_of_text, file);

    if ((ch = strstr(line_of_text, "soil-to-fertilizer map:")))
      targets.no_of_fertilizers = collect_types(
          &targets.fertilizers, targets.no_of_fertilizers, line_of_text, file);

    if ((ch = strstr(line_of_text, "fertilizer-to-water map:")))
      targets.no_of_water = collect_types(&targets.water, targets.no_of_water,
                                          line_of_text, file);

    if ((ch = strstr(line_of_text, "water-to-light map:")))
      targets.no_of_light = collect_types(&targets.light, targets.no_of_light,
                                          line_of_text, file);

    if ((ch = strstr(line_of_text, "light-to-temperature map:")))
      targets.no_of_temp = collect_types(
          &targets.temperature, targets.no_of_temp, line_of_text, file);

    if ((ch = strstr(line_of_text, "temperature-to-humidity map:")))
      targets.no_of_humidity = collect_types(
          &targets.humidity, targets.no_of_humidity, line_of_text, file);

    if ((ch = strstr(line_of_text, "humidity-to-location map:")))
      targets.no_of_location = collect_types(
          &targets.location, targets.no_of_location, line_of_text, file);
  }

  fclose(file);

  for (int i = 0; i < seeds.no_of_refs; i++) {
    printf("seed ref: %ld\n", seeds.list[i]);
  }

  for (int sei = 0; sei < seeds.no_of_refs; sei += 2) {
    for (unsigned long true_seed_no = seeds.list[sei];
         true_seed_no < (seeds.list[sei] + seeds.list[sei + 1]);
         true_seed_no++) {
      unsigned long result = true_seed_no;
      seeds.total_count++;

      result = go_through(result, targets.soils, targets.no_of_soils);
      result =
          go_through(result, targets.fertilizers, targets.no_of_fertilizers);
      result = go_through(result, targets.water, targets.no_of_water);
      result = go_through(result, targets.light, targets.no_of_light);
      result = go_through(result, targets.temperature, targets.no_of_temp);
      result = go_through(result, targets.humidity, targets.no_of_humidity);
      result = go_through(result, targets.location, targets.no_of_location);

      if (lowest_location == -1 || result < lowest_location)
        lowest_location = result;
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &ending_time);

  long elapsed_time_ns =
      (ending_time.tv_sec - starting_time.tv_sec) * 1000000000 +
      (ending_time.tv_nsec - starting_time.tv_nsec);

  printf("\nThe lowest location found was: %ld\n", lowest_location);
  printf("The target location is below 24261546\n");
  printf("Total count of seeds was: %ld\n", seeds.total_count);
  printf("Running time: %.3lf ms, %.2lf seconds\n",
         (double)elapsed_time_ns / 1000000,
         (double)elapsed_time_ns / 1000000000);

  free(seeds.list);
  free(targets.soils);
  free(targets.fertilizers);
  free(targets.water);
  free(targets.light);
  free(targets.temperature);
  free(targets.humidity);
  free(targets.location);

  return 0;
}