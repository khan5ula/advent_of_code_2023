#ifndef SEEDS_H
#define SEEDS_H

#include <stdio.h>

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

int collect_seeds(unsigned long** seeds, int no_of_seed_refs, char* ch);
int collect_types(unsigned long** collection,
                  const int org_number,
                  char* line_of_text,
                  FILE* file);
unsigned long go_through(const unsigned long source_no,
                         const unsigned long* collection,
                         const int count);
void trim_seeds(unsigned long** seeds, const int no_of_seed_refs);
void free_targets(targets_t* targets);
unsigned long go_through_from_seeds(seeds_t* seeds, targets_t targets);

#endif