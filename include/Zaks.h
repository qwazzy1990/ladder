
#ifndef _ZAKS_
#define _ZAKS_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ladder.h"
#include "utilities.h" 

#define appendPerm(perms,  perm, n) \
    appendPerm_real(&perms, perm, n)
void suffixSize(int n, int* suffixVector);
int getLen(int* a);
void zaks(int** perms, int* perm, int n, int* suffixSizes, int idx);
void appendPerm_real(int*** perms, int* perm, int n);
void genLaddersFromPerms(int** perms, int n);
#endif