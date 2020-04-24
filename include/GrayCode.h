#ifndef _GRAY_
#define _GRAY_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "Zaks.h"
#include "Sjt.h"
#include "Heaps.h"
#include "lexicographic.h"
#include "ladder.h"

//main driver
void grayCodeDriver(int n);

//sub drivers
List* sjtGrayCodeDriver(int n);
List* heapsGrayCodeDriver(int n);
List* zaksGrayCodeDriver(int n);
List* lexGrayCodeDriver(int n);


//List Drivers
List* genCanonicalLadders(List* perm, int n);

//calculate the difference of minimal change, insert or delete
int calMinChangeDifferential(Bar** firstBars, Bar** secondBars, int lenOne, int lenTwo);
List* getMinChangeDifferential(List* ladders);


//calculate the number of swaps from one ladder to the next
int calSwapDifferential(Ladder* l1, Ladder* l2);
List* getSwapDifferentials(List* ladders);

//calculates the avergage amount of insertion/deletions OR the average amount of swaps per algoritm
double calculateAverageAmntOfDiff(List* differentials);

void printDiff(List* sjt, List* heaps, List* zaks, List* lex, int mode);

void runGrayCode( int upperLimit );

void saveLaddersOfAllPerms(List* metaList, List* allPerms, int n);

Ladder* getCanonicalLadder(Ladder* currLadder, List* candiateLadders, int* currDiff);
List* canonicalList(List* metaList);
#endif