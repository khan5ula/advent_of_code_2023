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
  clock_t starting_time = clock();

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

  trim_seeds(&seeds, no_of_seed_refs);

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

  clock_t time_difference = clock() - starting_time;
  int msec = time_difference * 1000 / CLOCKS_PER_SEC;

  printf("\nThe lowest location found was: %ld\n", lowest_location);
  printf("The target location is below 24261546\n");
  printf("Total count of seeds was: %ld\n", total_count_of_seeds);
  printf("Running time: %d ms, about %d s\n", msec, msec / 1000);

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

int collect_seeds(unsigned long** seeds, int no_of_seed_refs, char* ch) {
  int count = no_of_seed_refs;
  ch = strtok(ch, " ");

  while (ch) {
    if (isdigit(ch[0])) {
      if (count % INITIAL_SIZE == 0)
        *seeds =
            realloc(*seeds, (count + INITIAL_SIZE) * sizeof(unsigned long));
      (*seeds)[count++] = atol(ch);
    }
    ch = strtok(NULL, " ");
  }

  return count;
}

int collect_types(unsigned long** collection,
                  const int org_number,
                  char* line_of_text,
                  FILE* file) {
  int count = org_number;

  while (fgets(line_of_text, 1024, file) && !feof(file)) {
    if (isdigit(line_of_text[0]) || isdigit(line_of_text[1])) {
      char* ch = strtok(line_of_text, " ");

      while (ch) {
        if (isdigit(ch[0]) || isdigit(ch[1])) {
          if (count % INITIAL_SIZE == 0)
            *collection = realloc(
                *collection, (count + INITIAL_SIZE) * sizeof(unsigned long));
          (*collection)[count++] = atol(ch);
        }
        ch = strtok(NULL, " ");
      }
    } else if (line_of_text[0] == '\n') {
      break;
    }
  }

  return count;
}

unsigned long go_through(const unsigned long source_no,
                         const unsigned long* collection,
                         const int count) {
  unsigned long result = source_no;

  for (int index = 0; (index + 2) < count; index += 3) {
    const unsigned long dest = collection[index];
    const unsigned long src = collection[index + 1];
    const unsigned long range = collection[index + 2];

    if (result >= src && result <= src + range) {
      result =
          (dest > src) ? result + abs(dest - src) : result - abs(dest - src);
      break;
    }
  }

  return result;
}

void trim_seeds(unsigned long** seeds, const int no_of_seed_refs) {
  for (int sei = 0; sei < no_of_seed_refs; sei += 2) {
    for (int inner_i = sei + 2; inner_i <= no_of_seed_refs; inner_i += 2) {
      const unsigned long first = (*seeds)[sei];
      const unsigned long first_range = (*seeds)[sei + 1];
      const unsigned long second = (*seeds)[inner_i];
      const unsigned long second_range = (*seeds)[inner_i + 1];

      if (first > second && second + second_range > first) {
        (*seeds)[inner_i + 1] = first - second;
      } else if (second > first && first + first_range > second) {
        (*seeds)[sei + 1] = second - first;
      }
    }
  }
}
