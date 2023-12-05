#include <ctype.h>
#include <stdio.h>
#include <string.h>

int get_sum_from_string(char* string);
int find_digit_from_text(const char* string, int* sourceIndex);
int compare_substring(const char* substring);

int main(void) {
  FILE* file;
  char line[1024] = {0};
  int sum = 0;
  int lineno = 0;  // debug

  if ((file = fopen("source.txt", "r")) == NULL) {
    perror("Couldn't open the file");
    return 1;
  }

  while (fgets(line, 1024, file) != NULL || !feof(file)) {
    printf("line: %d\n", ++lineno);
    sum += get_sum_from_string(line);
  }

  fclose(file);

  printf("Sum: %d\n", sum);

  return 0;
}

int get_sum_from_string(char* string) {
  int index = 0;
  int found = 0;
  int result = 0;
  int digits[20] = {-1};

  while (index < strlen(string)) {
    if (isdigit(string[index])) {
      digits[found++] = string[index++] - '0';
      printf("found digit: %d\n", string[index - 1] - '0');
    } else {
      int stringAsNum = find_digit_from_text(string, &index);
      if (stringAsNum > -1) {
        digits[found++] = stringAsNum;
        printf("found digit as text: %d\n", stringAsNum);
      }
    }
  }

  if (found > 1) {
    printf("adding %d and %d to the result\n", 10 * digits[0],
           digits[found - 1]);
    result += 10 * digits[0];
    result += digits[found - 1];
  } else if (found == 1) {
    printf("adding %d and %d to the result\n", 10 * digits[0], digits[0]);
    result += 10 * digits[0];
    result += digits[0];
  }

  return result;
}

int find_digit_from_text(const char* string, int* sourceIndex) {
  int num = -1;
  int subIndex = 0;
  char substring[100] = {'\0'};

  while (*sourceIndex < strlen(string)) {
    if (!isdigit(string[*sourceIndex])) {
      substring[subIndex++] = string[(*sourceIndex)++];
      substring[subIndex] = '\0';
      num = compare_substring(substring);
      if (num > -1) {
        if (*sourceIndex > 0) {
          // The ending char may be shared with next alpha form digit
          (*sourceIndex)--;
        }
        break;
      }
    } else {
      break;
    }
  }

  return num;
}

int compare_substring(const char* substring) {
  int result = -1;

  const char* digitsAsStrings[] = {"one", "two",   "three", "four", "five",
                                   "six", "seven", "eight", "nine"};
  const int numbers[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

  for (int i = 0; i < 9; i++) {
    const char* found = strstr(substring, digitsAsStrings[i]);

    if (found != NULL) {
      result = numbers[i];
      break;
    }
  }

  return result;
}