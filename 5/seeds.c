#include "seeds.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 10

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
    for (int inner_i = sei + 2; inner_i < no_of_seed_refs; inner_i += 2) {
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
