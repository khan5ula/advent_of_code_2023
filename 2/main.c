#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_game_id(char* line);
void trim_id_from_line(const char* source, char* result);
void countOfColors(char* game, int* reds, int* blues, int* greens);

int main(void) {
  int red_limit = 12;
  int blue_limit = 14;
  int green_limit = 13;

  FILE* file;
  char line[1024] = {0};
  int sum_of_ids = 0;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  while (fgets(line, 1024, file) != NULL || !feof(file)) {
    int id = find_game_id(line);
    int blocks_are_below_limit = 1;
    trim_id_from_line(line, line);
    char* game = strtok(line, ";");

    while (game != NULL) {
      int reds = 0;
      int blues = 0;
      int greens = 0;
      countOfColors(game, &reds, &blues, &greens);

      if (reds > red_limit || blues > blue_limit || greens > green_limit) {
        blocks_are_below_limit = 0;
      }

      game = strtok(NULL, ";");
    }

    if (blocks_are_below_limit == 1)
      sum_of_ids += id;
  }

  fclose(file);

  printf("Sum of ID's: %d\n", sum_of_ids);

  return 0;
}

int find_game_id(char* line) {
  int id = -1;

  if (strlen(line) < 7) {
    perror("malformatted string");
    return -1;
  }

  // Check for three-digit ID's
  if (isdigit(line[5]) && isdigit(line[6]) && isdigit(line[7])) {
    id = 100 * (line[5] - '0') + 10 * (line[6] - '0') + (line[7] - '0');
    // Check for two-digit ID's
  } else if (isdigit(line[5]) && isdigit(line[6])) {
    id = 10 * (line[5] - '0') + (line[6] - '0');
    // Check for one-digit ID's
  } else if (isdigit(line[5])) {
    id = line[5] - '0';
  }

  return id;
}

void trim_id_from_line(const char* source, char* result) {
  int destIndex = 0;
  int start_copying = 0;

  for (int srcIndex = 0; srcIndex < strlen(source); srcIndex++) {
    if (source[srcIndex] == ':' && source[srcIndex + 1] == ' ') {
      start_copying = 1;
      srcIndex = srcIndex + 2;
    }

    if (start_copying) {
      result[destIndex++] = source[srcIndex];
    }
  }

  result[destIndex] = '\0';
}

void countOfColors(char* game, int* reds, int* blues, int* greens) {
  char color;
  int count = 0;

  for (int i = 0; i < strlen(game); i++) {
    if (isdigit(game[i])) {
      // Check for one-digit counts
      if (isspace(game[i + 1])) {
        color = game[i + 2];
        count = game[i] - '0';
        // Check for two-digit counts
      } else if (isdigit(game[i + 1])) {
        color = game[i + 3];
        count = 10 * (game[i] - '0') + game[i + 1] - '0';
        i++;
      }

      if (color == 'r') {
        *reds = count;
      } else if (color == 'b') {
        *blues = count;
      } else if (color == 'g') {
        *greens = count;
      }
    }
  }
}
