
#ifndef _ZAKS_
#define _ZAKS_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ladder.h"
#include "utilities.h" 
#include "LinkedListAPI.h"

void suffixSize(int n, int* suffixVector);
int getLen(int* a);
void zaks(List* perms, int* perm, int n, int* suffixSizes, int idx);
#endif