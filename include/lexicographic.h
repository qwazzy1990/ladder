#ifndef LEX
#define LEX

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ladder.h"


void orderedPerms(int** perms, int* perm, int nFactorial, int n, int size);

int getSwapIndex(int* perm, int currIndex, int size);

void sortRightPart(int* perm, int start, int end);


#endif
