#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isSymbol(char character);

#define LINES 140

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
    char digits[10] = {'\0'};
    int startingIndexOfDigit = 0;
    int endingIndexOfDigit = 0;
    int consecutiveDigits = 0;

    for (int character = 0; character < strlen(lines[lineno]); character++) {
      int digit = -1;
      int finished = 1;

      if (isdigit(lines[lineno][character])) {
        if (consecutiveDigits == 0) {
          startingIndexOfDigit = character;
        }
        endingIndexOfDigit = character;

        digits[consecutiveDigits++] = lines[lineno][character];
        digits[consecutiveDigits] = '\0';
      } else {
        if (consecutiveDigits > 0) {
          // A full digit has been found
          digit = atoi(digits);

          // check left of the same line
          if (startingIndexOfDigit > 0) {
            if (isSymbol(lines[lineno][startingIndexOfDigit - 1]) == 0) {
              sum += digit;
              finished = 0;
            }
          }

          // check right of the same line
          if (finished == 1 && endingIndexOfDigit < strlen(lines[lineno]) - 1) {
            if (isSymbol(lines[lineno][endingIndexOfDigit + 1]) == 0) {
              sum += digit;
              finished = 0;
            }
          }

          // check the line above if exists
          if (finished == 1 && lineno > 0) {
            for (int i = startingIndexOfDigit - 1; i < endingIndexOfDigit + 2;
                 i++) {
              if (isSymbol(lines[lineno - 1][i]) == 0) {
                sum += digit;
                finished = 0;
              }
            }
          }

          // check line below if exists
          if (finished == 1 && lineno < LINES - 1) {
            for (int i = startingIndexOfDigit - 1; i < endingIndexOfDigit + 2;
                 i++) {
              if (isSymbol(lines[lineno + 1][i]) == 0) {
                sum += digit;
              }
            }
          }

          digits[0] = '\0';
          startingIndexOfDigit = 0;
          endingIndexOfDigit = 0;
          consecutiveDigits = 0;
        }
      }
    }
  }

  printf("The sum is: %d\n", sum);

  fclose(file);
  return 0;
}

/**
 * Checks whether the received character is a symbol
 * other than '.'. Returns 0 if is, returns 1 if not.
 */
int isSymbol(char character) {
  int result = 1;

  if (!isdigit(character)) {
    int potentialSymbol = character;

    /**
     * Check whether the character is in the ascii range
     * of the allowed symbols
     */
    if ((potentialSymbol > 32 && potentialSymbol < 46) ||
        potentialSymbol == 47 ||
        (potentialSymbol > 57 && potentialSymbol < 65) ||
        (potentialSymbol > 90 && potentialSymbol < 97) ||
        (potentialSymbol > 122 && potentialSymbol < 127)) {
      result = 0;
    }
  }

  return result;
}