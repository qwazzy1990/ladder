#ifndef _LADDER_
#define _LADDER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAXROWS 500
typedef struct bar
{
    //route the bar belongs to
    int routeNum;

    int barNum;
    //it's values = the inversion it uninverts: route number plus some other value < routeNum
    int vals[2];

    //if the bar is set in the ladde. Default ladder is a dummy ladder
    bool set;
} Bar;

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

/**
 * Gets the row >= 0 such that the row is the min 
 * of all empty rows in the ladder.
 */
int getFirstAvailableRow(Ladder* l, int currRow, int col);

/*
    Returns a bar associated with the value 'n' in the ladder of
    l. Primarily used for outputting human readable information.
 */
Bar* getBar(Ladder* l, int n);
//printer

char* printBar(Bar* b);
void printLadder(Ladder* l);

/*
    Used for hashing a string representation 
    of a ladder to check if said ladder has been
    generated already or not.
 */
char* ladderToString(void* l);
//delete

//find turn bar
//set row and col

//Sets a bar in the ladder with rowNum, colNum, routeNum, and valTwo. 
//bar->vals[0] = routeNum. Therefore valOne is not needed as a param
void setBar(Bar* bar, int barNum, int routeNum, int valTwo);

//right swap
//Swaps a bar at currRow, currCol to row col
//row and col are the destination: row is equal to the row of the upper neighbor of the upper neighbor of the
//current val. Col is equal to the currCol + 1.
//if the cell is able to be moved into then apply case 1: 
//else apply case 2
void rightSwap(Ladder* l, int currRow, int currCol, int row, int col, int* mode);

/*For fixing empty rows */
void readjustLadder(Ladder* l, int start, int end, int offset);
void shiftLadderDown(Ladder* l, int start, int end);
void shiftLadderUp(Ladder* l, int start, int end, int offset);
void fixLadder(Ladder * l);



/*For shifting the children of val in rightSwap
This function shifts the children up the ladder. I.e. given
child, ch, if ch's row is 'x' in state 1, then ch's row will be
x-n after shiftChildren is applied. */
void shiftChildren(Ladder* l, int val, int offset);

/*
    For shifting the children of val in rightSwap.
    This function shifts the children down the ladder.
    I.e. given child, ch, if ch's row is x in state 1 then ch's
    row will be x+n after shiftChildrenDown is applied.
 */
void shiftChildrenDown(Ladder* l, int val, int offset);

/**
 * Calculates the offset, or the amount of rows, the child is allowed
 * to shift.
 */
int calculateChildOffset(Ladder* l, int leftChild);

void makeRowEmpty(Ladder* l, int row);

int getRowToGo(Ladder* l, int val);


/*These two functions are for right swapping */
int getUpperNeighbor(Ladder* l, int n);
int getRightNeighbor(Ladder* l, int n);
int getLeftNeighbor(Ladder* l, int n);

/*These two functions are for left swapping - reverse engineering */
int getLowerNeighbor(Ladder* l, int n);
int getLeftNeighbor(Ladder* l, int n);

/*These functions return the left and right child of val.
They are required for readjusting the ladder when a right swap
has been made */
int getLeftChild(Ladder* l, int val);
int getRightChild(Ladder* l, int val);

int getRowIndex(Ladder* l, int n);
int getColIndex(Ladder* l, int n);

/*Get the clean level of the ladder.
Clean level is defined as the route in the ladder such 
that there is no bar above said route that belongs to a route of lesser value.
For example, if the clean level is 3, then every bar above route 3 will be part of route 4, 5, 6, ...
and every bar below route 3 will be part of route 2,1,0,-1.... However, because 3 is the clean level, the
value before 3 will not be clean. I.e. 2 is the "dirty" level. This means that route 2 has a bar above it belonging
to route 1 and/or 0 and/or -1.... */
int getCleanLevel(Ladder* l);

/**
 * A turnbar is defined as a value, turn-value, in the root->ladder such that 
 * the value has exactly two neighbors. One neighbor exists in the same collum as turn-value
 * and is "above" turn-value. I.e. neighbor-1 has a row index < turn-value's row index.
 * The other neighbor is in turn value's collumn +1 and has a row index < turn value's row index
 * but greater than neighbor 1's row index.
 */
int getFirstTurnBar(Ladder* root);

int getStartOfRoute(Ladder* l, int route, int* arr);

int getEndOfRoute(Ladder* l, int route, int* arr);


int getDepth(Ladder* l);


void leftSwap(Ladder* l, Ladder* clone);

//Finds a row to go to based on the current start row and start col
void findRowToGo(Ladder* l, int startRow, int startCol);
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

void findAllChildren(Ladder* l, int cleanLevel, int level);
void driver(int* perm, int size);


/*Main algorithm: Get all children */

#endif