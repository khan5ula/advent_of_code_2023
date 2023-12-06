#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10

int collect_seeds(unsigned long** seeds, int no_of_seeds, char* ch);
int collect_types(unsigned long** collection,
                  int org_number,
                  char* line_of_text,
                  FILE* file);
unsigned long go_through(const unsigned long* seeds,
                         const unsigned long* collection,
                         int sei,
                         int count);

int main(void) {
  FILE* file;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  char line_of_text[1024] = {'\0'};

  int no_of_seeds = 0;
  int no_of_soils = 0;
  int no_of_fertilizers = 0;
  int no_of_water = 0;
  int no_of_light = 0;
  int no_of_temp = 0;
  int no_of_humidity = 0;
  int no_of_location = 0;

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

    // collect seeds entries
    if ((ch = strstr(line_of_text, "seeds:")))
      no_of_seeds = collect_seeds(&seeds, no_of_seeds, ch);

    // collect soils entries
    if ((ch = strstr(line_of_text, "seed-to-soil map:")))
      no_of_soils = collect_types(&soils, no_of_soils, line_of_text, file);

    // collect fertilizers entries
    if ((ch = strstr(line_of_text, "soil-to-fertilizer map:")))
      no_of_fertilizers =
          collect_types(&fertilizers, no_of_fertilizers, line_of_text, file);

    // collect water entries
    if ((ch = strstr(line_of_text, "fertilizer-to-water map:")))
      no_of_water = collect_types(&water, no_of_water, line_of_text, file);

    // collect light entries
    if ((ch = strstr(line_of_text, "water-to-light map:")))
      no_of_light = collect_types(&light, no_of_light, line_of_text, file);

    // collect temperature entries
    if ((ch = strstr(line_of_text, "light-to-temperature map:")))
      no_of_temp = collect_types(&temperature, no_of_temp, line_of_text, file);

    // collecth humidity entries
    if ((ch = strstr(line_of_text, "temperature-to-humidity map:")))
      no_of_humidity =
          collect_types(&humidity, no_of_humidity, line_of_text, file);

    // collect location entries
    if ((ch = strstr(line_of_text, "humidity-to-location map:")))
      no_of_location =
          collect_types(&location, no_of_location, line_of_text, file);
  }

  fclose(file);

  for (int sei = 0; sei < no_of_seeds; sei++) {
    seeds[sei] = go_through(seeds, soils, sei, no_of_soils);
    seeds[sei] = go_through(seeds, fertilizers, sei, no_of_fertilizers);
    seeds[sei] = go_through(seeds, water, sei, no_of_water);
    seeds[sei] = go_through(seeds, light, sei, no_of_light);
    seeds[sei] = go_through(seeds, temperature, sei, no_of_temp);
    seeds[sei] = go_through(seeds, humidity, sei, no_of_humidity);
    seeds[sei] = go_through(seeds, location, sei, no_of_location);

    if (seeds[sei] < lowest_location || lowest_location > -1) {
      lowest_location = seeds[sei];
    } else if (lowest_location == -1) {
      lowest_location = seeds[sei];
    }
  }

  printf("The lowest location found was: %ld\n", lowest_location);

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

int collect_seeds(unsigned long** seeds, int no_of_seeds, char* ch) {
  int count = no_of_seeds;
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
                  int org_number,
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

unsigned long go_through(const unsigned long* seeds,
                         const unsigned long* collection,
                         int sei,
                         int count) {
  unsigned long result = seeds[sei];

  for (int tyi = 0; (tyi + 2) < count; tyi += 3) {
    if (seeds[sei] >= collection[tyi + 1] &&
        seeds[sei] <= collection[tyi + 1] + collection[tyi + 2]) {
      result = (collection[tyi] > collection[tyi + 1])
                   ? result + (collection[tyi] - collection[tyi + 1])
                   : result - (collection[tyi + 1] - collection[tyi]);
      break;
    }
  }

  return result;
}