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

//calculate the difference of minimal change
int calMinChangeDifferential(Bar** firstBars, Bar** secondBars, int lenOne, int lenTwo);
List* getMinChangeDifferential(List* ladders);
#endif