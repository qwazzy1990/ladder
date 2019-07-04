#ifndef _LADDER_
#define _LADDER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAXROWS 1000
typedef struct bar
{
    //route the bar belongs to
    int routeNum;

    //it's current row
    int currRow;
    //it's current collumn
    int currCol;

    //it's previous row: if it has not been swapped set to currRow
    int prevRow;

    //it's previous col: if bar has not been swapped set to currCol
    int prevCol;

    //it's values = the inversion it uninverts: route number plus some other value < routeNum
    int vals[2];

    //if the bar is set in the ladde. Default ladder is a dummy ladder
    bool set;
} Bar;

typedef struct ladder
{
    Bar** ladder;
    int numRows;
    int numCols;
} Ladder;


//constructor
//Creates a Ladder*, l, with numCols = size-1, numRows = MAXROWS, and ladder = NULL 
Ladder* newLadder(int* perm, int size);

void initLadder(Ladder* l);


//printer
//delete

//find turn bar
//set row and col

//Sets a bar in the ladder with rowNum, colNum, routeNum, and valTwo. 
//bar->vals[0] = routeNum. Therefore valOne is not needed as a param
void setBar(Ladder* l, int rowNum, int colNum, int routeNum, int valTwo);
//Sets a dummyBar. Default values are -1 -1 set=false
void setDummy(Ladder* l, int rowNum, int colNum);

//right swap
//Swaps a bar at currRow, currCol to row col
void rightSwap(Ladder* l, int currRow, int currCol, int row, int col);

void leftSwap(Ladder* l, int currRow, int currCol, int row, int col);

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


void driver(int* perm, int size);
#endif