#include <ctype.h>
#include <stdio.h>
#include <string.h>

int getSumFromString(char* string);

int main(void) {
  FILE* file;
  char line[1024] = {0};
  int sum = 0;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("Couldn't open the file");
    return 1;
  }

  while (fgets(line, 1024, file) != NULL || !feof(file)) {
    sum += getSumFromString(line);
  }

  fclose(file);

  printf("Sum: %d\n", sum);

  return 0;
}

int getSumFromString(char* string) {
  int result = 0;
  int first_found = 0;
  int last_found = 0;

  for (int i = 0, j = strlen(string) - 1; i < strlen(string); i++, j--) {
    if (!first_found && isdigit(string[i])) {
      result += 10 * (string[i] - '0');
      first_found = 1;
    }

    if (!last_found && isdigit(string[j])) {
      result += string[j] - '0';
      last_found = 1;
    }
  }

  return result;
}