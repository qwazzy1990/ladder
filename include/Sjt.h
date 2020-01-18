#ifndef __SJT__
#define __SJT__


#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"

#define LEFT false
#define RIGHT true
int searchArr(int* a, int n, int mobile);

int getMobile(int* a, bool* dir, int n);

void printOnePerm(int** perms, int *a, bool *dir, int n, int count);

void genPermsSJT(int** perms, int n);

int fact(int n);


//Failed sjt algorithm, still interesting algorithm for generating all perms
void sjt(int *perm, int count, int max, int size, bool flag);

void genPermsSJTReverse(int** perms, int* perm, int n, int *arr, bool *direction);

void adjustPerm(int* perm, int n, int* arr, bool* direction);

void _swap(int* v1, int* v2);

int _getIndex(int* perm, int n, int val);


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


//checks if there is an upper neighbor of the place where the current bar will go
bool hasUpperNeighbor(Ladder* l, int n, int row, int col, int upperBound);


//checks if there is a lower neighbor of the palce where the current bar will go
bool hasLowerNeighbor(Ladder* l, int n, int row, int col, int loweBound);


//inserts  a bar at the row within the range of reserved rows based on the val.
//the column is there
void insertAt(Ladder* l, int val, int* reserved, int col);

#endif