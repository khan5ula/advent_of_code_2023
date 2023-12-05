#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_game_id(char* line);
void trim_id_from_line(const char* source, char* result);
void countOfColors(char* game, int* reds, int* blues, int* greens);

int main(void) {
  FILE* file;
  char line[1024] = {0};
  int result = 0;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  while (fgets(line, 1024, file) != NULL || !feof(file)) {
    int reds_max = 0;
    int blues_max = 0;
    int greens_max = 0;

    trim_id_from_line(line, line);
    char* game = strtok(line, ";");

    while (game != NULL) {
      int reds = 0;
      int blues = 0;
      int greens = 0;
      countOfColors(game, &reds, &blues, &greens);

      if (reds > reds_max)
        reds_max = reds;

      if (blues > blues_max)
        blues_max = blues;

      if (greens > greens_max)
        greens_max = greens;

      game = strtok(NULL, ";");
    }

    result += reds_max * blues_max * greens_max;
  }

  fclose(file);
  printf("Result: %d\n", result);

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
