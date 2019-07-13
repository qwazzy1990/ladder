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

Ladder* cloneLadder(Ladder * l);

void initLadder(Ladder* l);

void createRoot(Ladder* l, int* perm, int size, int currRow);

//Getters
int getLargestIndex(int* perm, int size);

int getFirstAvailableRow(Ladder* l, int currRow, int col);

Bar* getBar(Ladder* l, int n);
//printer

char* printBar(Bar* b);
void printLadder(Ladder* l);

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

/*For shifting rectangular regions of ladder in rightSwap */
void shiftRectangle(Ladder* l, int w, int x, int y, int z, int offset);

/*For shifting the children of a bar in rightSwap */
void shiftChildren(Ladder* l, int val, int offset);

void shiftChildrenDown(Ladder* l, int val, int offset);
int calculateChildOffset(Ladder* l, int leftChild);

void makeRowEmpty(Ladder* l, int row);

int getRowToGo(Ladder* l, int val);


/*These two functions are for right swapping */
int getUpperNeighbor(Ladder* l, int n);
int getRightNeighbor(Ladder* l, int n);

/*These two functions are for left swapping - reverse engineering */
int getLowerNeighbor(Ladder* l, int n);
int getLeftNeighbor(Ladder* l, int n);

int getLeftChild(Ladder* l, int val);
int getRightChild(Ladder* l, int val);

int getRowIndex(Ladder* l, int n);
int getColIndex(Ladder* l, int n);

/*Get the clean level of the ladder */
int getCleanLevel(Ladder* l);

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
bool emptyRow(Ladder* l, int row);
bool emptyRegion(int startRow, int startCol, int endRow, int endCol);

bool isRightSwappable(Ladder* l, int val);

bool canBeAddedToRow(Ladder* l, int row, int col);

void findAllChildren(Ladder* l, int cleanLevel);
void driver(int* perm, int size);


/*Main algorithm: Get all children */

void findAllChildren(Ladder* l, int cleanLevel);
#endif