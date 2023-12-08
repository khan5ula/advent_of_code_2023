#ifndef SEEDS_H
#define SEEDS_H

#include <stdio.h>

typedef struct seeds_t {
  unsigned long* list;
  int count;
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

targets_t init_targets();
void collect_seeds(seeds_t* seeds, FILE* file, char* line_of_text);
int seed_refs(unsigned long** seeds, int no_of_seed_refs, char* ch);
void collect_targets(targets_t* targets, FILE* file, char* line_of_text);
int collect_target(unsigned long** collection,
                   const int org_number,
                   char* line_of_text,
                   FILE* file);
unsigned long go_through(const unsigned long source_no,
                         const unsigned long* collection,
                         const int count);
unsigned long inverse_go_through(const unsigned long digit,
                                 const unsigned long* collection,
                                 const int count);
unsigned long go_through_from_seeds(seeds_t* seeds, targets_t targets);
unsigned long find_max_value(seeds_t seeds);
unsigned long go_through_from_location(seeds_t* seeds, targets_t targets);
void trim_seeds(unsigned long** seeds, const int no_of_seed_refs);
void free_targets(targets_t* targets);
void print_results(unsigned long lowest_location, seeds_t seeds, long time);

#endif