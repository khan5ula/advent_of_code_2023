#include "seeds.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_SIZE 10

int collect_seeds(unsigned long** collection, int no_of_seed_refs, char* ch);
int collect_types(unsigned long** collection,
                  const int org_number,
                  char* line_of_text,
                  FILE* file);
unsigned long go_through(const unsigned long source_no,
                         const unsigned long* collection,
                         const int count);
void trim_seeds(unsigned long** seeds, const int no_of_seed_refs);

int main(int argc, char* argv[]) {
  if (argc < 1) {
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

  int no_of_seed_refs = 0;
  int no_of_soils = 0;
  int no_of_fertilizers = 0;
  int no_of_water = 0;
  int no_of_light = 0;
  int no_of_temp = 0;
  int no_of_humidity = 0;
  int no_of_location = 0;
  unsigned long total_count_of_seeds = 0;

  unsigned long* seeds = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* soils = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* fertilizers = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* water = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* light = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* temperature = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* humidity = malloc(INITIAL_SIZE * sizeof(unsigned long));
  unsigned long* location = malloc(INITIAL_SIZE * sizeof(unsigned long));

  unsigned long lowest_location = -1;

  while (fgets(line_of_text, 1024, file)) {
    char* ch;

    if ((ch = strstr(line_of_text, "seeds:")))
      no_of_seed_refs = collect_seeds(&seeds, no_of_seed_refs, ch);

    if ((ch = strstr(line_of_text, "seed-to-soil map:")))
      no_of_soils = collect_types(&soils, no_of_soils, line_of_text, file);

    if ((ch = strstr(line_of_text, "soil-to-fertilizer map:")))
      no_of_fertilizers =
          collect_types(&fertilizers, no_of_fertilizers, line_of_text, file);

    if ((ch = strstr(line_of_text, "fertilizer-to-water map:")))
      no_of_water = collect_types(&water, no_of_water, line_of_text, file);

    if ((ch = strstr(line_of_text, "water-to-light map:")))
      no_of_light = collect_types(&light, no_of_light, line_of_text, file);

    if ((ch = strstr(line_of_text, "light-to-temperature map:")))
      no_of_temp = collect_types(&temperature, no_of_temp, line_of_text, file);

    if ((ch = strstr(line_of_text, "temperature-to-humidity map:")))
      no_of_humidity =
          collect_types(&humidity, no_of_humidity, line_of_text, file);

    if ((ch = strstr(line_of_text, "humidity-to-location map:")))
      no_of_location =
          collect_types(&location, no_of_location, line_of_text, file);
  }

  fclose(file);

  for (int i = 0; i < no_of_seed_refs; i++) {
    printf("seed ref: %ld\n", seeds[i]);
  }

  for (int sei = 0; sei < no_of_seed_refs; sei += 2) {
    for (unsigned long true_seed_no = seeds[sei];
         true_seed_no < (seeds[sei] + seeds[sei + 1]); true_seed_no++) {
      unsigned long result = true_seed_no;
      total_count_of_seeds++;

      result = go_through(result, soils, no_of_soils);
      result = go_through(result, fertilizers, no_of_fertilizers);
      result = go_through(result, water, no_of_water);
      result = go_through(result, light, no_of_light);
      result = go_through(result, temperature, no_of_temp);
      result = go_through(result, humidity, no_of_humidity);
      result = go_through(result, location, no_of_location);

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
  printf("Total count of seeds was: %ld\n", total_count_of_seeds);
  printf("Running time: %.3lf ms, %.2lf seconds\n",
         (double)elapsed_time_ns / 1000000,
         (double)elapsed_time_ns / 1000000000);

  free(seeds);
  free(soils);
  free(fertilizers);
  free(water);
  free(light);
  free(temperature);
  free(humidity);
  free(location);

  return 0;
}