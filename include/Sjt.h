#ifndef __SJT__
#define __SJT__


#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int searchArr(int* a, int n, int mobile);

int getMobile(int* a, bool* dir, int n);

void printOnePerm(int** perms, int *a, bool *dir, int n, int count);

void printPerms(int** perms, int n);

int fact(int n);

#endif