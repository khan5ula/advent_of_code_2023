#ifndef SEEDS_H
#define SEEDS_H

#include <stdio.h>

typedef struct seeds_t {
  long* list;
  int count;
} seeds_t;

typedef struct targets_t {
  long* soils;
  long* fertilizers;
  long* water;
  long* light;
  long* temperature;
  long* humidity;
  long* location;

  int no_of_soils;
  int no_of_fertilizers;
  int no_of_water;
  int no_of_light;
  int no_of_temp;
  int no_of_humidity;
  int no_of_location;
} targets_t;

targets_t init_targets();
void collect_seeds(seeds_t* seeds, FILE* file, char* line_of_text);
int seed_refs(long** seeds, int no_of_seed_refs, char* ch);
void collect_targets(targets_t* targets, FILE* file, char* line_of_text);
int collect_target(long** collection,
                   const int org_number,
                   char* line_of_text,
                   FILE* file);
long go_through(const long source_no, const long* collection, const int count);
long inverse_go_through(const long digit,
                        const long* collection,
                        const int count);
long go_through_from_seeds(seeds_t* seeds, targets_t targets);
long find_max_value(seeds_t seeds);
long go_through_from_location(seeds_t* seeds, targets_t targets);
void trim_seeds(long** seeds, const int no_of_seed_refs);
void free_targets(targets_t* targets);
void print_results(long lowest_location, seeds_t seeds, long time);

#endif