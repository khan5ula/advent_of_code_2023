#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES 140

int extractDigits(const char* source, const int begin, const int end);

int main(void) {
  FILE* file;
  char lines[LINES][1024] = {{'\0'}};
  int sum = 0;

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("couldn't open the file");
    return 1;
  }

  int lineno = 0;

  while (fgets(lines[lineno++], 1024, file) != NULL && lineno < LINES)
    ;

  for (lineno = 0; lineno < LINES; lineno++) {
    for (int character = 0; character < strlen(lines[lineno]); character++) {
      if (lines[lineno][character] == '*') {
        int digitsNextToStar[2] = {0};
        int found = 0;
        int sIndexOfD, eIndexOfD = -1;

        // check left of the same line
        if (character > 1) {
          char* line = lines[lineno];
          if (isdigit(line[character - 1])) {
            sIndexOfD = character - 1;
            eIndexOfD = character - 1;
            for (int i = eIndexOfD; i >= 0; i--) {
              if (isdigit(line[i])) {
                sIndexOfD = i;
              } else {
                break;
              }
            }
            digitsNextToStar[found++] =
                extractDigits(line, sIndexOfD, eIndexOfD);
          }
        }

        sIndexOfD = -1;
        eIndexOfD = -1;

        // check right of the same line
        if (character < strlen(lines[lineno]) - 1) {
          char* line = lines[lineno];
          if (isdigit(line[character + 1])) {
            sIndexOfD = character + 1;
            eIndexOfD = character + 1;
            for (int i = eIndexOfD; i < strlen(line) - 1; i++) {
              if (isdigit(line[i])) {
                eIndexOfD = i;
              } else {
                break;
              }
            }
            digitsNextToStar[found++] =
                extractDigits(line, sIndexOfD, eIndexOfD);
          }
        }

        sIndexOfD = -1;
        eIndexOfD = -1;

        // check the line above
        if (lineno > 0) {
          char* line = lines[lineno - 1];
          for (int index = 0; index < strlen(line); index++) {
            if (isdigit(line[index])) {
              if (sIndexOfD == -1)
                sIndexOfD = index;
              eIndexOfD = index;
            } else {
              if (sIndexOfD > -1) {
                if ((abs(sIndexOfD - character) < 2 ||
                     (abs(eIndexOfD - character) < 2))) {
                  digitsNextToStar[found++] =
                      extractDigits(line, sIndexOfD, eIndexOfD);
                }
              }
              sIndexOfD = -1;
              eIndexOfD = -1;
            }
          }
        }

        // check the line below
        if (lineno < LINES - 1) {
          char* line = lines[lineno + 1];
          for (int index = 0; index < strlen(line); index++) {
            if (isdigit(line[index])) {
              if (sIndexOfD == -1)
                sIndexOfD = index;
              eIndexOfD = index;
            } else {
              if (sIndexOfD > -1) {
                if ((abs(sIndexOfD - character) < 2 ||
                     (abs(eIndexOfD - character) < 2))) {
                  digitsNextToStar[found++] =
                      extractDigits(line, sIndexOfD, eIndexOfD);
                }
              }
              sIndexOfD = -1;
              eIndexOfD = -1;
            }
          }
        }

        if (found == 2)
          sum += (digitsNextToStar[0] * digitsNextToStar[1]);
      }
    }
  }

  printf("The sum is: %d\n", sum);

  fclose(file);
  return 0;
}

int extractDigits(const char* source, const int begin, const int end) {
  int result = -1;
  char digits[5] = {'\0'};
  int sourceIndex = begin;
  int destIndex = 0;

  for (; sourceIndex <= end; sourceIndex++) {
    digits[destIndex++] = source[sourceIndex];
  }

  if (digits[destIndex] != '\0') {
    digits[destIndex] = '\0';
  }

  result = atoi(digits);

  return result;
}