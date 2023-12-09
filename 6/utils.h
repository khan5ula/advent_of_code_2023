#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

int get_digits(char* source, int times[], int length, char* searchBy);
int count_successful_ways(int times[], int distances[], int length);
int count_race_wins(int time, int distance);

#endif