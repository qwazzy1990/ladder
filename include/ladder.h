#ifndef _LADDER_
#define _LADDER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.h"


#define MAXROWS 500
typedef struct bar
{
    //route the bar belongs to
    int routeNum;

    int barNum;
    //it's values = the inversion it uninverts: route number plus some other value < routeNum
    int vals[2];

    int numSwaps;

    
    //if the bar is set in the ladde. Default ladder is a dummy ladder
    bool set;

    // the route numbers the bars can cross
    int routesCrossed[10];

    //The number of times the bar crosses that route
    int numberOfTimesCrossed[10];

    //currentlySwapped
    bool isSwapped;
} Bar;


typedef struct DegenerativeTriadicSubsequence{

    int lower[2];
    int mid[2];
    int top[2];

    bool isRotated;
    int num;
} DTS;

typedef struct DegenerativeTriadicSubSequenceArray
{
    int amnt;
    DTS** dts;
}DTSA;
typedef struct ladder
{
    int** ladder;
    int numRows;
    int numCols;
    int numBars;
    int depth;
    int cleanLevel;
    Bar** bars;
} Ladder;


//constructor
//Creates a Ladder*, l, with numCols = size-1, numRows = MAXROWS, and ladder = NULL 
Ladder* newLadder(int size);
Bar* newBar( void );
Bar* cloneBar(Bar* b);

/*
    Clones a ladder, l, and returns its clone.
    Used to preserve previous state of a ladder when returning from recursive calls in 
    find all children
 */
Ladder* cloneLadder(Ladder * l);

/**
 * Initializes a ladder to default values.
 */
void initLadder(Ladder* l);

/*
    Sets the Ladder* l to the root ladder based on the permutation, perm.
    The root ladder is defined as the only ladder in the set of 
    all optimal ladders given a specific permutation, perm, such that the root has
    a clean level of 1. No other ladder in the set has a clean level of 1
 */
void createRoot(Ladder* l, int* perm, int size, int currRow);

/***DESTROYERS */

void delstroyBar(void* bar);

void destroyLadder(void * l);

void destroyClone(void * clone);

//Getters
/**
 * Gets the index in perm that contains the largest value.
 */
int getLargestIndex(int* perm, int size);


/*
    Returns a bar associated with the value 'n' in the ladder of
    l. Primarily used for outputting human readable information.
 */
Bar* getBar(Ladder* l, int n);
//printer

char* printBar(Bar* b);
void printLadder(Ladder* l);
void printPerm(int *perm, int size);
char* ladderToString(void * data);


//delete

//find turn bar
//set row and col

//Sets a bar in the ladder with rowNum, colNum, routeNum, and valTwo. 
//bar->vals[0] = routeNum. Therefore valOne is not needed as a param
void setBar(Bar* bar, int barNum, int routeNum, int valTwo);

void setRoutesCrossed(Ladder* l, int* perm, int size);

void setTimesCrossed(Bar* b, int levelCrossed);

//right swap
//Swaps a bar at currRow, currCol to row col
//row and col are the destination: row is equal to the row of the upper neighbor of the upper neighbor of the
//current val. Col is equal to the currCol + 1.
//if the cell is able to be moved into then apply case 1: 
//else apply case 2
void rightSwap(Ladder* l, int val);

/*For fixing empty rows */
void fixLadder(Ladder * l);


/*
    For shifting the children of val in rightSwap.
    This function shifts the children down the ladder.
    I.e. given child, ch, if ch's row is x in state 1 then ch's
    row will be x+n after shiftChildrenDown is applied.
 */
void shiftChildrenDown(Ladder* l, int val, int offset);






/*These two functions are for right swapping */
int getUpperNeighbor(Ladder* l, int n);
int getRightNeighbor(Ladder* l, int n);

/*These two functions are for left swapping - reverse engineering */
int getLowerNeighbor(Ladder* l, int n);

/*These functions return the left and right child of val.
They are required for readjusting the ladder when a right swap
has been made */
int getLeftChild(Ladder* l, int val);
int getRightChild(Ladder* l, int val);

int getRowIndex(Ladder* l, int n);
int getColIndex(Ladder* l, int n);





int getStartOfRoute(Ladder* l, int route, int* arr);

int getEndOfRoute(Ladder* l, int route, int* arr);


int getDepth(Ladder* l);


void leftSwap(Ladder* l, Ladder* clone);


//left swap
    //find row to go

//swap vals

void swapVals(int* valOne, int* valTwo);
//set new row/col


//getter - get row/col prevRow/prevCol
//add row
//delete row
//shift down
//reverse back

//emptyRow
//checks if l->ladder[row] is empty
bool emptyRow(Ladder* l, int row);

//emptyCell
//checks if l->ladder[row][col] is set or not
bool emptyCell(Ladder* l, int row, int col);

bool isRightSwappable(Ladder* l, int val);

bool canBeAddedToRow(Ladder* l, int row, int col);

/*
    Checks if val can be the active bar value 
    in l->ladder. The active bar is defined as 
    the bar that is upward visible from clean level k
    and is the rightmost value. Thus, if val is the value 
    of the active bar and k were to = 5, then val would be 
    upward visible from route 5 and it would be the rightmost 
    value that is upward visible from 5.
 */
bool canBeActiveBar(Ladder* l, int val, int k);

bool isUpwardVisible(Ladder * l, int val, int route);

void findAllChildren( Ladder* l, int cleanLevel, int level, DTSA* dts);
void driver(int* perm, int size);


/*Main algorithm: Get all children */



/**The following functions pertain to answering the following reasearch questions
 * Q1: Given a permutation, can you determine the number of ladders in its set of efficient ladder lotteries.
 * Hypothesis: The number of ladders in the set is equal to the number of degenerative subsequences of size 3. 
 * A degenerative subequence of size 3 is defined as follows. Assume N is the largest value, N-X is the second largest 
 * value and N-Y is the third largest/smallest value in the degenerative subsequence. Assume X < Y. Then a degenerative subsequence means that N is to
 * the left of N-X which is to the left of N-Y in the permutation.
 */

int countDegenerativeSubsequences(int * perm, int size);


/*Displays the number of times each bar was swapped. */

void displaySwapCount(Ladder * l);

int factorial(int n);
void getInput(int **perm, char *s);



///DTS Functions


void rotateDTS(DTSA* dts, int l1, int l2, int m1, int m2, int t1, int t2, int numDts);


void setDTSs(DTSA* dts, int* perm, int numDts, int n);

void printDTS(DTS* d);
void printDTSA(DTSA* dts);

void printDtsAsBinString(DTSA* dts);

void freeDts(DTSA* dts, int n);
#endif