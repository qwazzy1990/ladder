#ifndef LEX
#define LEX

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ladder.h"
#include "LinkedListAPI.h"

void orderedPerms(List* perms, int* perm, int nFactorial, int n, int size);

int getSwapIndex(int* perm, int currIndex, int size);

void sortRightPart(int* perm, int start, int end);


#endif
