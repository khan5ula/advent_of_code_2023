#include "seeds.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 20

targets_t init_targets() {
  targets_t targets = {.soils = malloc(INITIAL_SIZE * sizeof(long)),
                       .fertilizers = malloc(INITIAL_SIZE * sizeof(long)),
                       .water = malloc(INITIAL_SIZE * sizeof(long)),
                       .light = malloc(INITIAL_SIZE * sizeof(long)),
                       .temperature = malloc(INITIAL_SIZE * sizeof(long)),
                       .humidity = malloc(INITIAL_SIZE * sizeof(long)),
                       .location = malloc(INITIAL_SIZE * sizeof(long)),

                       .no_of_soils = 0,
                       .no_of_fertilizers = 0,
                       .no_of_water = 0,
                       .no_of_light = 0,
                       .no_of_temp = 0,
                       .no_of_humidity = 0,
                       .no_of_location = 0};

  return targets;
}

void collect_seeds(seeds_t* seeds, FILE* file, char* line_of_text) {
  char* ch;
  if ((ch = strstr(line_of_text, "seeds:")))
    seeds->count = seed_refs(&seeds->list, seeds->count, ch);
}

int seed_refs(long** seeds, int no_of_seed_refs, char* ch) {
  int count = no_of_seed_refs;
  ch = strtok(ch, " ");
  char* end;

  while (ch) {
    if (isdigit(ch[0])) {
      if (count % INITIAL_SIZE == 0)
        *seeds = realloc(*seeds, (count + INITIAL_SIZE) * sizeof(long));
      (*seeds)[count++] = strtol(ch, &end, 10);
    }
    ch = strtok(NULL, " ");
    end = NULL;
  }

  return count;
}

void collect_targets(targets_t* targets, FILE* file, char* line_of_text) {
  char* ch;

  if ((ch = strstr(line_of_text, "seed-to-soil map:")))
    targets->no_of_soils = collect_target(&targets->soils, targets->no_of_soils,
                                          line_of_text, file);

  if ((ch = strstr(line_of_text, "soil-to-fertilizer map:")))
    targets->no_of_fertilizers = collect_target(
        &targets->fertilizers, targets->no_of_fertilizers, line_of_text, file);

  if ((ch = strstr(line_of_text, "fertilizer-to-water map:")))
    targets->no_of_water = collect_target(&targets->water, targets->no_of_water,
                                          line_of_text, file);

  if ((ch = strstr(line_of_text, "water-to-light map:")))
    targets->no_of_light = collect_target(&targets->light, targets->no_of_light,
                                          line_of_text, file);

  if ((ch = strstr(line_of_text, "light-to-temperature map:")))
    targets->no_of_temp = collect_target(
        &targets->temperature, targets->no_of_temp, line_of_text, file);

  if ((ch = strstr(line_of_text, "temperature-to-humidity map:")))
    targets->no_of_humidity = collect_target(
        &targets->humidity, targets->no_of_humidity, line_of_text, file);

  if ((ch = strstr(line_of_text, "humidity-to-location map:")))
    targets->no_of_location = collect_target(
        &targets->location, targets->no_of_location, line_of_text, file);
}

int collect_target(long** targets,
                   const int org_number,
                   char* line_of_text,
                   FILE* file) {
  int count = org_number;

  while (fgets(line_of_text, 1024, file) && !feof(file)) {
    if (isdigit(line_of_text[0]) || isdigit(line_of_text[1])) {
      char* ch = strtok(line_of_text, " ");
      char* end;

      while (ch) {
        if (isdigit(ch[0]) || isdigit(ch[1])) {
          if (count % INITIAL_SIZE == 0)
            *targets = realloc(*targets, (count + INITIAL_SIZE) * sizeof(long));
          (*targets)[count++] = strtol(ch, &end, 10);
        }
        ch = strtok(NULL, " ");
      }
    } else if (line_of_text[0] == '\n') {
      break;
    }
  }

  return count;
}

long go_through(const long source_no, const long* collection, const int count) {
  long result = source_no;

  for (int index = 0; index < count; index += 3) {
    const long dest = collection[index];
    const long src = collection[index + 1];
    const long range = collection[index + 2];

    if (result >= src && result <= src + range) {
      result =
          (dest > src) ? result + abs(dest - src) : result - abs(dest - src);
      break;
    }
  }

  return result;
}

long inverse_go_through(const long digit,
                        const long* collection,
                        const int count) {
  long result = digit;

  for (int index = 0; index < count; index += 3) {
    const long dest = collection[index];
    const long src = collection[index + 1];
    const long range = collection[index + 2];

    if (result >= dest && result <= dest + range) {
      result =
          (dest > src) ? result - abs(dest - src) : result + abs(dest - src);
      break;
    }
  }

  return result;
}

long go_through_from_seeds(seeds_t* seeds, targets_t targets) {
  long lowest = -1;

  for (int sei = 0; sei < seeds->count / 2; sei += 2) {
    for (long true_seed_no = seeds->list[sei];
         true_seed_no < (seeds->list[sei] + seeds->list[sei + 1]);
         true_seed_no++) {
      long result = true_seed_no;

      result = go_through(result, targets.soils, targets.no_of_soils);
      result =
          go_through(result, targets.fertilizers, targets.no_of_fertilizers);
      result = go_through(result, targets.water, targets.no_of_water);
      result = go_through(result, targets.light, targets.no_of_light);
      result = go_through(result, targets.temperature, targets.no_of_temp);
      result = go_through(result, targets.humidity, targets.no_of_humidity);
      result = go_through(result, targets.location, targets.no_of_location);

      if (result < lowest || lowest < 0)
        lowest = result;
    }
  }

  return lowest;
}

long find_max_value(seeds_t seeds) {
  long max = 0;

  for (int index = 0; index < seeds.count; index += 2) {
    if (seeds.list[index] + seeds.list[index + 1] > max)
      max = seeds.list[index] + seeds.list[index + 1];
  }

  return max;
}

long go_through_from_location(seeds_t* seeds, targets_t targets) {
  long max_value = find_max_value(*seeds);

  for (int digit = 0; digit <= max_value; digit++) {
    long result = digit;

    result =
        inverse_go_through(result, targets.location, targets.no_of_location);
    result =
        inverse_go_through(result, targets.humidity, targets.no_of_humidity);
    result =
        inverse_go_through(result, targets.temperature, targets.no_of_temp);
    result = inverse_go_through(result, targets.light, targets.no_of_light);
    result = inverse_go_through(result, targets.water, targets.no_of_water);
    result = inverse_go_through(result, targets.fertilizers,
                                targets.no_of_fertilizers);
    result = inverse_go_through(result, targets.soils, targets.no_of_soils);

    for (int index = 0; index < seeds->count; index += 2) {
      long seed_start = seeds->list[index];
      long seed_end = seed_start + seeds->list[index + 1];

      if (result >= seed_start && result <= seed_end) {
        return digit;
      }
    }
  }

  return -1;
}

void free_targets(targets_t* targets) {
  free(targets->soils);
  free(targets->fertilizers);
  free(targets->water);
  free(targets->light);
  free(targets->temperature);
  free(targets->humidity);
  free(targets->location);
}

void print_results(long lowest_location, seeds_t seeds, long time) {
  printf("\nThe lowest location found was: %ld\n", lowest_location);
  printf("The target location is below 24261546\n");
  printf("Running time: %.3lf ms, %.2lf seconds\n", (double)time / 1000000,
         (double)time / 1000000000);
}
