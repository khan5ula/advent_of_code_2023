#ifndef SEEDS_H
#define SEEDS_H

#include <stdio.h>

int collect_seeds(unsigned long** seeds, int no_of_seed_refs, char* ch);
int collect_types(unsigned long** collection,
                  const int org_number,
                  char* line_of_text,
                  FILE* file);
unsigned long go_through(const unsigned long source_no,const unsigned long* collection, const int count);
void trim_seeds(unsigned long** seeds, const int no_of_seed_refs);

#endif