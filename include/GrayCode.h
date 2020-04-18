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
int** sjtGrayCodeDriver(int n);
int** heapsGrayCodeDriver(int n);
int** zaksGrayCodeDriver(int n);
int** lexGrayCodeDriver(int n);


//List Drivers
List* genCanonicalLadders(int** perm, int n);

//calculate the difference of minimal change, insert or delete
int calMinChangeDifferential(Bar** firstBars, Bar** secondBars, int lenOne, int lenTwo);
List* getMinChangeDifferential(List* ladders);


//calculate the number of swaps from one ladder to the next
int calSwapDifferential(Ladder* l1, Ladder* l2);
List* getSwapDifferentials(List* ladders);

//calculates the avergage amount of insertion/deletions OR the average amount of swaps per algoritm
double calculateAverageAmntOfDiff(List* differentials);

#endif