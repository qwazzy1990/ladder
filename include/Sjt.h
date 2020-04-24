#ifndef __SJT__
#define __SJT__


#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"
#include "LinkedListAPI.h"


#define LEFT false
#define RIGHT true


int searchArr(int* a, int n, int mobile);

int getMobile(int* a, bool* dir, int n);

void printOnePerm(int** perms, int *a, bool *dir, int n, int count);

void genPermsSJT(List* perms, int * perm, int n, int* arr, bool* direction);
void adjustPerm(List* perms, int* perm, int n, int* arr, bool* direction);
void copyPerm(int *dest, int *src, int n);
void adjustPermReverse(int **perms, int *perm, int n, int *arr, bool *direction);
int fact(int n);

int _getIndex(int *perm, int n, int val);
void _swap(int *v1, int *v2);

//Failed sjt algorithm, still interesting algorithm for generating all perms
void sjt(int *perm, int count, int max, int size, bool flag);

void sjtLadder(Ladder* l, int n, int* arr, bool* direction);
//adds or removes a bar belonging to level 1 <= level <= n-1
void adjustLadder(Ladder* l, int* arr, int level, bool* direction);


//reserves the rows in a ladder for the bars of value n to 2
void reserveRows(int* reservations, int n);


//checks if there is a neighbor to the left of the current row 
//and col
bool hasLeftNeighbor(Ladder* l, int row, int col);

//checks if there is a neighbor to the left of the current row 
//and col
bool hasRightNeighbor(Ladder* l, int row, int col);
#endif