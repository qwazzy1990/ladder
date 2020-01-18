#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"
#include "LinkedListAPI.h"

/*Global variables: used for clean level */

int MAXVAL;
int ladderCount = 1;

bool DEBUG3 = true;
bool PRINT = false;

/***STATIC FUNCTIONS */
static void copyArray(int **write, int *read, int largestIndex, int size)
{
    int *temp = *write;
    int i = 0;
    forall(size)
    {
        if (x != largestIndex)
        {
            temp[i] = read[x];
            i++;
        }
    }
}

static int compareInts(const void *a, const void *b)
{
    int *f = (int *)a;
    int *g = (int *)b;
    return ((*f) - (*g));
}

void swapVals(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

Ladder *newLadder(int size)
{
    new_object(Ladder *, l, 1);

    l->ladder = NULL;
    l->bars = NULL;

    l->numRows = MAXROWS;
    l->numCols = size - 1;
    l->numBars = 0;
    l->depth = 0;
    return l;
}

/*Creates a clone of ladder l, used for reverse engineering a right swap */
Ladder *cloneLadder(Ladder *l)
{
    new_object(Ladder *, clone, 1);

    clone->numCols = l->numCols;
    clone->numRows = l->numRows;
    clone->numBars = l->numBars;
    clone->depth = l->depth;

    initLadder(clone);

    for (int i = 0; i < MAXROWS; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            clone->ladder[i][j] = l->ladder[i][j];
        }
    }

    forall(l->numBars)
    {
        clone->bars[x] = cloneBar(l->bars[x]);
    }
    return clone;
}

Bar *newBar(void)
{
    new_object(Bar *, b, 1);
    b->set = true;
    b->numSwaps = 0;
    forall(10)
    {
        b->routesCrossed[x] = -1;
        b->numberOfTimesCrossed[x] = 0;
    }
    return b;
}

Bar* cloneBar(Bar* b)
{
    new_object(Bar*, bb, 1);
    bb->set = b->set;
    bb->numSwaps = b->numSwaps;
    bb->vals[0] = b->vals[0];
    bb->vals[1] = b->vals[1];
    bb->routeNum = b->routeNum;
    bb->barNum = b->barNum;
    bb->isSwapped = b->isSwapped;
    forall(10)
    {
        bb->routesCrossed[x] = b->routesCrossed[x];
        bb->numberOfTimesCrossed[x] = b->numberOfTimesCrossed[x];
    }
    return bb;

}

/**DESTROYERS Free memory*/
void destroyBar(void *bar)
{
    if (bar == NULL)
        return;
    clear(bar);
}

void destroyLadder(void *l)
{
    if (l == NULL)
        return;
    Ladder *ll = (Ladder *)l;
    forall(ll->numRows)
    {
        clear(ll->ladder[x]);
    }
    clear(ll->ladder);

    forall(100)
    {
        destroyBar(ll->bars[x]);
    }
    clear(ll->bars);
    clear(ll);
}

void destroyClone(void *l)
{
    if (l == NULL)
        return;
    Ladder *ll = (Ladder *)l;
    forall(ll->numRows)
    {
        clear(ll->ladder[x]);
    }
    clear(ll->ladder);
    forall(100)destroyBar(ll->bars[x]);
    clear(ll->bars);
    clear(ll);
}

/*END DESTROYERS */

void initLadder(Ladder *l)
{
    l->ladder = calloc(MAXROWS, sizeof(int *));

    //Create the ladder
    forall(l->numRows)
    {
        l->ladder[x] = calloc(l->numRows, sizeof(int));
    }

    //Initialize the ladder
    for (int i = 0; i < l->numRows; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            l->ladder[i][j] = 0;
        }
    }

    //Create the array of bars
    l->bars = calloc(100, sizeof(Bar *));
}

void createRoot(Ladder *l, int *perm, int size, int currRow)
{
    /**
     * Base case: fix the completed root ladder and return
     */
    if (size == 1)
    {
        /*Fix the ladder by shifting every value as high as it can go */
        if (l->numBars == 0)
        {
            forall(l->numCols)
            {
                if(PRINT)
                    printf("[0 0] ");
            }
            if(PRINT)
                printf("\n");
            return;
        }
        l->depth = getDepth(l);
        fixLadder(l);
        l->cleanLevel = 1;
        return;
    }

    int largestIndex = getLargestIndex(perm, size);

    foreach (largestIndex, size)
    {
        //if an inversion is found based on the largest value in the array, create a bar in the ladder
        if (perm[largestIndex] > perm[x])
        {
            int col = x - 1;

            //create a bar for the inversion
            Bar *b = newBar();

            //set it's values
            setBar(b, l->numBars + 1, perm[largestIndex], perm[x]);

            //put it into the array
            l->bars[l->numBars] = b;

            //increase number of bars
            l->numBars++;

            //set the value in the table
            l->ladder[currRow][col] = l->numBars;
            currRow++;
        }
    }

    int *arr = calloc(size - 1, sizeof(int));
    /*Copy everything from the previous array into arr except the largest value. */
    copyArray(&arr, perm, largestIndex, size);
    /*Call create root, this time the next largest value will be used to generate bars */
    createRoot(l, arr, size - 1, currRow);
    free(arr);
}

/**GETTERS */
/*Returns the index that contains the largest value in perm */
int getLargestIndex(int *perm, int size)
{
    int largestVal = -1;
    int largestIndex = -1;
    forall(size)
    {
        if (perm[x] > largestVal)
        {
            largestVal = perm[x];
            largestIndex = x;
        }
    }

    return largestIndex;
}

Bar *getBar(Ladder *l, int n)
{
    forall(l->numBars)
    {
        Bar *b = l->bars[x];
        if (b->barNum == n)
        {
            return b;
        }
    }
    return NULL;
}

/*Printers */
char *printBar(Bar *b)
{
    if (b == NULL)
        return NULL;
    char *s = calloc(1000, sizeof(char));
    char temp[100];
    sprintf(temp, "%d  ", b->barNum);
    strcat(s, "Bar Number:");
    strcat(s, temp);
    sprintf(temp, "%d ", b->vals[0]);
    strcat(s, "Values:");
    strcat(s, temp);
    strcat(s, " ");
    sprintf(temp, "%d\n", b->vals[1]);
    strcat(s, temp);
    sprintf(temp, "%d", b->numSwaps);
    strcat(s, "Number of times the bar was swapped:");
    strcat(s, temp);
    strcat(s, "\n");
    strcat(s, "Routes Crossed:\n");
    int x = 0;
    while (b->routesCrossed[x] != -1)
    {
        sprintf(temp, "%d ", b->routesCrossed[x]);
        strcat(s, temp);
        sprintf(temp, "%d", b->numberOfTimesCrossed[x]);
        strcat(s, "Crossed ");
        strcat(s, temp);
        strcat(s, " times\n");
        x++;
    }
    return s;
}

void printLadder(Ladder *l)
{
    for (int i = 0; i <= l->depth + 1; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val == 0)
                printf("[0 0] ");
            else
            {
                Bar *b = getBar(l, val);

                printf(GREEN "[%d %d] " COLOR_RESET, b->vals[0], b->vals[1]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

char *ladderToString(void *data)
{
    Ladder *l = (Ladder *)data;
    int memSize = 1050;

    char *s = calloc(memSize, sizeof(char));
    for (int i = 0; i <= l->depth + 1; i++)
    {
        memSize += memSize;
        s = realloc(s, memSize * sizeof(char));
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val == 0)
                strcat(s, "[0 0] ");
            else
            {
                Bar *b = getBar(l, val);
                char *temp = calloc(50, sizeof(char));
                sprintf(temp, "%d", b->vals[0]);
                strcat(s, "[");
                strcat(s, temp);
                strcat(s, " ");
                free(temp);
                temp = calloc(50, sizeof(char));
                sprintf(temp, "%d", b->vals[1]);
                strcat(s, temp);
                free(temp);
                strcat(s, "] ");
            }
        }
        strcat(s, "\n");
    }
    strcat(s, "\n");
    return s;
}

//prints the permutation
void printPerm(int *perm, int size)
{
    forall(size)
    {
            printf("%d     ", perm[x]);
    }
        printf("\n");
}

/*Setters */
void setBar(Bar *bar, int barNum, int routeNum, int valTwo)
{
    bar->set = true;
    bar->barNum = barNum;
    bar->routeNum = routeNum;
    bar->vals[0] = routeNum;
    bar->vals[1] = valTwo;
}

void setRoutesCrossed(Ladder *l, int *perm, int size)
{
    for (int i = 0; i < l->numRows; i++)
        for (int j = 0; j < l->numCols; j++)
        {
            if (l->ladder[i][j] != 0)
            {
                Bar *b = getBar(l, l->ladder[i][j]);
                int count = 0;
                for (int k = size - 1; k >= 0; k--)
                {
                    if (perm[k] > b->routeNum)
                    {
                        b->routesCrossed[count] = perm[k];
                        count++;
                    }
                }
            }
        }
}

void setTimesCrossed(Bar *b, int levelCrossed)
{
    int x = 0;
    while (b->routesCrossed[x] != -1)
    {
        if (b->routesCrossed[x] == levelCrossed)
            b->numberOfTimesCrossed[x]++;

        x++;
    }
}

/**
 * Used to generate the next ladder in the set of ladders.
 * A right swap of val can occurr iff val has exactly 1 right neighbor, exactly 0 left neighbors
 * and exactly 1 upper neighbor. 
 * Swapping is done as follows. 
 * Let val = a, val's right neighbor be = to b and val's upper neighbor be c.
 * Before swap: a is the left child of b, and b is the right child of c. And b may or may not have 
 * a right sub-tree attached to it.
 * After swap: b is the left child of a, and c is the right child of b, and the previous right subtree of b
 * is now the right subtree of c
 */
void rightSwap(Ladder *l, int val)
{

    int upperNeighbor = getUpperNeighbor(l, val);

    //Get the rightNeighbor
    int rightNeighbor = getRightNeighbor(l, val);

    //get the row and collumn of the upper neighbor
    int upArr[2] = {-1, -1};
    upArr[0] = getRowIndex(l, upperNeighbor);
    upArr[1] = getColIndex(l, upperNeighbor);

    //get the row and collumn of the right neighbor
    int rightArr[2] = {-1, -1};
    rightArr[0] = getRowIndex(l, rightNeighbor);
    rightArr[1] = getColIndex(l, rightNeighbor);

    int currArr[2] = {-1, -1};
    currArr[0] = getRowIndex(l, val);
    currArr[1] = getColIndex(l, val);

    shiftChildrenDown(l, getRightChild(l, rightNeighbor), 2);

    swapVals(&(l->ladder[upArr[0]][upArr[1]]), &(l->ladder[currArr[0] + 1][currArr[1] + 1]));
    swapVals(&(l->ladder[currArr[0]][currArr[1]]), &(l->ladder[rightArr[0]][rightArr[1]]));

    fixLadder(l);
}

/*Reverse engineers right swap:
Copy the clone, which is the same as l before l underwent a right swap, back to the main ladder, l,
in order to reverse engineer the right swap*/
void leftSwap(Ladder *l, Ladder *clone)
{
    l->numBars = clone->numBars;
    l->numCols = clone->numCols;
    l->numRows = clone->numRows;
    l->depth = clone->depth;
    for (int i = 0; i < MAXROWS; i++)
        for (int j = 0; j < l->numCols; j++)
            l->ladder[i][j] = clone->ladder[i][j];
}

//return the row index of n. -1 if fail
int getRowIndex(Ladder *l, int n)
{
    int row = -1;
    for (int i = 0; i < l->numRows; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            if (l->ladder[i][j] == n)
            {
                row = i;
            }
        }
    }
    return row;
}

//Return the column index of n.
//-1 if fail
int getColIndex(Ladder *l, int n)
{
    int col = -1;
    for (int i = 0; i < l->numRows; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            if (l->ladder[i][j] == n)
            {
                col = j;
            }
        }
    }
    return col;
}

//Gets the upper neighbor of value n where the upper
//neighbor, v, is defined as a value > 0 in the ladder with
//v's collumn value = to n's collumn value
//and v's row value x, where 0 <= x < n's row value
int getUpperNeighbor(Ladder *l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);

    if (row <= 0)
        return -1;
    if (col < 0)
        return -1;

    for (int i = row - 1; i >= 0; i--)
    {
        if (l->ladder[i][col] != 0)
        {
            return l->ladder[i][col];
        }
    }
    return -1;
}

//Gets the top right neighbor of a value, n, in the table
//Top right neighbor is defined as a value > 0 in a cell that is 1 collumn to the right of n's colloumn,
//and x rows above n's row where 0 <= x < n's row
int getRightNeighbor(Ladder *l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);
    if (row <= 0)
        return -1;
    if (col < 0 || col >= l->numCols - 1)
        return -1;

    row--;
    col++;
    for (int i = row; i >= 0; i--)
    {
        if (l->ladder[i][col] != 0)
        {
            return l->ladder[i][col];
        }
    }
    return -1;
}

/*Gets the lower neighbor of n. The lower neighbor is defined by any value in the ladder
!= to 0 and in the same collumn as n and with a row index > n's row index and <= the depth of the ladder */
int getLowerNeighbor(Ladder *l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);
    if (row >= l->depth)
        return -1;
    if (col < 0)
        return -1;
    row++;
    for (int i = row; i <= l->depth; i++)
    {
        if (l->ladder[i][col] != 0)
        {
            return l->ladder[i][col];
        }
    }
    return -1;
}

/*
    Returns the left child of val. Left child is defined as the value in the cell
    in the ladder at val's row+1 and val's col-1.
    Return -1 if no such value exist
 */
int getLeftChild(Ladder *l, int val)
{
    if (val == 0 || val == -1)
        return -1;
    int rowIndex = getRowIndex(l, val) + 1;
    int colIndex = getColIndex(l, val) - 1;
    if (colIndex == -1)
        return -1;

    int child = l->ladder[rowIndex][colIndex];
    return child;
}
/*
    Returns the right child of val. Right child is defined as the value in the cell
    in the ladder at val's row+1 and val's col+1.
    Return -1 if no such value exist
 */
int getRightChild(Ladder *l, int val)
{
    if (val == 0 || val == -1)
        return -1;
    int rowIndex = getRowIndex(l, val) + 1;
    int colIndex = getColIndex(l, val) + 1;
    if (colIndex >= l->numCols)
        return -1;
    int child = l->ladder[rowIndex][colIndex];
    return child;
}

/**
 * Readjusts the ladder to ensure that each value in the ladder is as high up
 * the ladder as possible.
 */
void fixLadder(Ladder *l)
{
    int end = l->depth + 10;
    //Start iterating from row 0
    for (int i = 1; i <= end; i++)
    {
        //Start iterating inner loop from right to left.
        //Ensures there is no error in the readjustment.
        for (int j = l->numCols - 1; j >= 0; j--)
        {
            if (l->ladder[i][j] != 0)
            {
                int val = l->ladder[i][j];

                int temp = getRowIndex(l, val) - 1;

                /**When a value is non-0, find how high it can go */
                while (canBeAddedToRow(l, temp, j))
                {
                    temp--;
                } //end while
                temp++;

                l->ladder[i][j] = 0;

                l->ladder[temp][j] = val;
            } //end if

        } //end for

    } //end for

    //recalculate the depth
    l->depth = getDepth(l);
} //end func

/*Only for reversing the ladder in leftSwap */
void shiftChildrenDown(Ladder *l, int val, int offset)
{
    /*If the value is 0 */
    if (val == 0 || val == -1)
        return;

    /*Case 1: there are no more children. Val is not as far left/right as possible*/
    if ((getLeftChild(l, val) == 0) && (getRightChild(l, val) == 0))
    {
        goto Base_Case;
    }
    /*Case two: there are no children. Val is as far right and left as possible? Random error*/
    if ((getLeftChild(l, val) == -1) && (getRightChild(l, val) == -1))
    {
        goto Base_Case;
    }

    /*Case three: value is as far left as possible - has no chilren */
    if ((getLeftChild(l, val) == -1) && (getRightChild(l, val) == 0))
    {
        goto Base_Case;
    }
    /*Case four: child is as far right as possibl. has no children */
    if ((getLeftChild(l, val) == 0) && (getRightChild(l, val) == -1))
    {
        goto Base_Case;
    }

    /*Recursively call function on left child and right child. */
    shiftChildrenDown(l, getLeftChild(l, val), offset);
    shiftChildrenDown(l, getRightChild(l, val), offset);

Base_Case:
{
    int colIndex = getColIndex(l, val);
    int rowIndex = getRowIndex(l, val);
    l->ladder[rowIndex + offset][colIndex] = val;
    l->ladder[rowIndex][colIndex] = 0;
    return;
}
}

void driver(List *list, int *perm, int size)
{
    if (size <= 0)
    {
        printf("Enter a permutation\n");
        return;
    }
    //create a ladder
    Ladder *l = newLadder(size);

    //initialize it
    initLadder(l);

    //Generate root.
    createRoot(l, perm, size, 0);
    int *arr = calloc(size, sizeof(int));
    forall(size)
    {
        arr[x] = perm[x];
    }

    qsort(arr, size, sizeof(int), compareInts);

    setRoutesCrossed(l, arr, size);
    //Initialzie MAXVAL to be the largest value in the permutation.
    //This ensures that in findAllChildren, when  the value of the cleanlevel
    ///parameter is greater than any value in the array, there won't be a route
    //to be traversed. And the algorithm will go to case 2
    MAXVAL = arr[size - 1];
    free(arr);

    //Call find all children with clean level 1 because l is currently the root.
    //The only ladder in the set with a clean level of 1.
    findAllChildren(list, l, 1, 0);
    if(PRINT)
    printf("The number of degenerative subsequences is %d\n", countDegenerativeSubsequences(perm, size));
    displaySwapCount(l);

    char* s = toString(list);
    if(PRINT)
    printf("%s\n", s);
    free(s);

    destroyLadder(l);
}

void findAllChildren(List *list, Ladder *l, int cleanLevel, int level)
{
    if (l == NULL)
        return;
    if (l->numBars == 0)
        return;

    if(PRINT){
        printf(RED "Clean Level:%d\n" COLOR_RESET, cleanLevel);
        printf(YELLOW "Depth:%d\n" COLOR_RESET, level);

        printf(CYAN "Ladder Number:%d\n" COLOR_RESET, ladderCount);
        printf(MAGENTA "Height:%d\n" COLOR_RESET, l->depth + 1);
    }
    ladderCount++;
    if(PRINT)
        printLadder(l);

    Ladder *clone = cloneLadder(l);
    insertBack(list, cloneLadder(l));

    int y = MAXVAL;

    /*While each route ,y, >= to the clean level, find a bar that can be right swappable */
    while (y >= cleanLevel)
    {
        /*Get the start and end of route y*/
        int start[2] = {-1, -1};
        int end[2] = {-1, -1};
        getStartOfRoute(l, y, start);
        getEndOfRoute(l, y, end);

        /*If the route exists then */
        if (start[0] != -1)

            /*Begin at the start of the route and go to the end */
            for (int i = start[0]; i <= end[0]; i++)
            {
                for (int j = start[1]; j <= end[1]; j++)
                {
                    int val = l->ladder[i][j];
                    if (val == 0)
                        continue;

                    /*Get the bar and check f it's route number is equal to y*/
                    Bar *b = getBar(l, val);
                    if (b->routeNum == y)
                    {
                        /*Get the lower neighbor of the value. Check if it can be righ swapped */
                        int lowerNeighbor = getLowerNeighbor(l, val);

                        if (isRightSwappable(l, lowerNeighbor))
                        {
                            /*for debugging */
                            Bar *bb = getBar(l, lowerNeighbor);
                            bb->numSwaps++;
                            setTimesCrossed(bb, y);
                            /*Swap the bar*/
                            rightSwap(l, lowerNeighbor);
                            //clear(bars);

                            /*Recursive call with clean level = to b->routeNum+1*/
                            findAllChildren(list, l, b->routeNum + 1, level + 1);
                            /*Reset to previous state, before right swap */
                            leftSwap(l, clone);
                        } //end if
                    }     //end if
                }         //end for
            }             //end for

        y--;
    } //End while

    //For when there is no such route >= the clean level in the ladder.
    //Only occurs when the parameter, cleanLevel, = MAX val of permutation + 1.
    //The cleanLevel of the permutation will never be > MAX val +1
    //Get the start and end of the route = to the MAX val of the cleanLevel
    int start[2] = {-1, -1};
    int end[2] = {-1, -1};
    getStartOfRoute(l, cleanLevel - 1, start);
    getEndOfRoute(l, cleanLevel - 1, end);
    if (start[0] != -1)
        //If there is no such route, y, >= to the clean level then for
        //i = start of route of cleanLevel-1 to end of route of cleanLevel -1 do
        //same thing as previous double for loops
        //for
        for (int i = start[0]; i <= end[0]; i++)
        {
            //for
            for (int j = start[1]; j <= end[1]; j++)
            {
                int val = l->ladder[i][j];
                if (val == 0)
                    continue;
                Bar *b = getBar(l, val);
                if (b->routeNum == y)
                {
                    int lowerNeighbor = getLowerNeighbor(l, val);

                    //The lowerNeighbor has to be rightSwappable and be an active bar.
                    if (isRightSwappable(l, lowerNeighbor) && canBeActiveBar(l, lowerNeighbor, y))
                    {
                        Bar *bb = getBar(l, lowerNeighbor);
                        bb->numSwaps++;
                        setTimesCrossed(bb, y);

                        rightSwap(l, lowerNeighbor);

                        findAllChildren(list, l, cleanLevel, level + 1);
                        leftSwap(l, clone);
                    }
                }
            }
        }
    destroyClone(clone);
}

bool isRightSwappable(Ladder *l, int val)
{

    if (val <= 0)
        return false;
    int rowIndex = getRowIndex(l, val);
    int colIndex = getColIndex(l, val);

    //if val is as far right as it ca go
    if (colIndex >= l->numCols - 1)
        return false;

    int left;
    int right;

    //if there is a collumn to the left of val's collumn
    if (colIndex > 0)
    {
        left = colIndex - 1;
    }
    else
    {
        left = -1;
    }

    //get the collumn to the right of val's collumn
    right = colIndex + 1;

    //rCount = rowCount; start at values rowIndex -1
    int rCount = rowIndex - 1;
    if (rCount <= 0)
        return false;
    //count the number of values above and to the right of val
    int rightCount = 0;

    //case if there is a collumn to the left of val

    //if
    if (left != -1)
    {
        //continue until you find a directlt vlaue above val
        //while
        while (l->ladder[rCount][colIndex] == 0)
        {
            //if there is a value above and to the left of val, then val cannot be right swapped
            if (l->ladder[rCount][left] != 0)
                return false;
            if (l->ladder[rCount][right] != 0)
            {
                rightCount++;
            }
            if (l->ladder[rCount][colIndex] != 0)
            {
                break;
            }
            rCount--;

            //if there is no value above val
            if (rCount < 0)
                return false;
        } //end while
    }     //endif

    //else
    else
    {
        //continue until you find a directlt vlaue above val
        while (l->ladder[rCount][colIndex] == 0)
        {
            //get the number of values above and to the right of value, v.
            if (l->ladder[rCount][right] != 0)
            {
                rightCount++;
            }
            if (l->ladder[rCount][colIndex] != 0)
            {
                break;
            }
            rCount--;

            //if there is no value above val
            if (rCount < 0)
                return false;
        } //end while

    } //end else

    //if there is not exactly one value above and to the right of val, then return false
    if (rightCount != 1)
        return false;

    //If there is exactly one value above and to the right of val and exactly one value directly above val
    //and exactly no values above and to the left of val, then val can be right swapped
    return true;

} //end func

/*Gets the first row and collumn of the route number in the ladder. */
int getStartOfRoute(Ladder *l, int routeNum, int *arr)
{
    for (int i = 0; i <= l->depth; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {

            if (l->ladder[i][j] != 0)
            {
                int barVal = l->ladder[i][j];
                //check the route number the barVal belongs to
                Bar *b = getBar(l, barVal);
                if (b->routeNum == routeNum)
                {
                    arr[0] = i;
                    arr[1] = j;
                    return 0;
                }
            }
        }
    }
    return -1;
}

/*Gets the last row and collumn of routeNum in l. Sets the values to arr */
int getEndOfRoute(Ladder *l, int routeNum, int *arr)
{
    for (int i = 0; i <= l->depth; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            if (l->ladder[i][j] != 0)
            {
                int barVal = l->ladder[i][j];
                Bar *b = getBar(l, barVal);
                if (b->routeNum == routeNum)
                {
                    arr[0] = i;
                    arr[1] = j;
                }
            }
        }
    }
    return 0;
}

/*
    Checks whether or not val can be the active bar.
    The active bar is defined as the rightmost bar that 
    is upward visible from route k. Upward visible means that, given val's collumn, N,
    in the ladder, val's left endpoint val's right endpoint are the lowest
    endpoints of any bar in the ladder that exist in collumn N-1, N and N+1 
 */
bool canBeActiveBar(Ladder *l, int val, int k)
{
    //Clone the ladder to not mess up the original ladder
    Ladder *clone = cloneLadder(l);

    //rightswap the value in the clone
    //FIX ME
    rightSwap(clone, val);

    int start[2] = {-1, -1};
    int end[2] = {-1, -1};

    //get the start and end of the route, k, to set the range of traversal
    getStartOfRoute(clone, k, start);
    getEndOfRoute(clone, k, end);

    //traverse from the right bottom of the route to the left top of the route
    //in order to find the rightmost upward visible bar from the route
    for (int i = end[0]; i >= start[0]; i--)
    {
        for (int j = end[1]; j >= start[1]; j--)
        {
            //don't waste time checking empty cell
            if (clone->ladder[i][j] == 0)
                continue;

            //get the bar assoiated with non empty cell
            Bar *b = getBar(clone, clone->ladder[i][j]);

            //check the bar's route number and see if it is equal to level k
            if (b->routeNum == k)
            {
                //if it is, then you know that the clone->ladder[i][j] belongs
                //to route k. Therefore get its upper neighbor
                int upperNeighbor = getUpperNeighbor(clone, clone->ladder[i][j]);
                if (upperNeighbor == -1 || upperNeighbor == 0)
                    continue;

                //If val is the rightmost upward visible bar along route k,
                //then val is the active bar in the clone. Therefore return true.
                if (isUpwardVisible(clone, upperNeighbor, k) && upperNeighbor == val)
                {
                    destroyClone(clone);
                    return true;
                }
                else if (isUpwardVisible(clone, upperNeighbor, k) && upperNeighbor != val)
                {
                    destroyClone(clone);
                    return false;
                }
                else
                    continue;
            }
        }
    }
    destroyClone(clone);
    return false;
}

/* 
    Checks if val is upwardVisible from route.
    Upward visible means that, given val's collumn, N,
    in the ladder, val's left endpoint val's right endpoint are the lowest
    endpoints of any bar in the ladder that exist in collumn N-1, N and N+1 */
bool isUpwardVisible(Ladder *l, int val, int route)
{
    if (val == -1 || val == 0)
        return false;

    /*Get the row and collumn index of val */
    int rowIndex = getRowIndex(l, val);
    int colIndex = getColIndex(l, val);

    /*Get the collumn to the left of val's column. */

    int left = colIndex - 1;

    int right;

    /*Get the collumn to the right of colIndex */
    if (colIndex != l->numCols - 1)
    {
        right = colIndex + 1;
    }
    /*If no such collumn, set to -1 */
    else
    {
        right = -1;
    }

    /*If there is a collumn to the left, check to see if there is a bar below val and above the route
    in said collumn */
    if (left != -1)
    {
        for (int i = rowIndex; i < l->depth; i++)
        {
            int leftVal = l->ladder[i][left];
            if (leftVal != 0)
            {
                Bar *b = getBar(l, leftVal);
                /*If the bar below the rowIndex of val does not belong to the route, 
                then val cannot be upward visible from the route */
                if (b->vals[0] != route && b->vals[1] != route)
                    return false;
                else
                    break;
            }
        }
    }

    /*If there is a collumn to the right, check to see if there is a bar below val and above
    the route in said collumn */
    if (right != -1)
    {
        for (int i = rowIndex; i < l->depth; i++)
        {
            int rightVal = l->ladder[i][right];
            if (rightVal != 0)
            {
                Bar *b = getBar(l, rightVal);
                /*If the bar below the rowIndex of val does not belong to the route, 
                then val cannot be upward visible from the route */
                if (b->vals[0] != route && b->vals[1] != route)
                    return false;
                else
                    break;
            }
        }
    }

    /*Get the downward neighbor of val. That is the bar below val in the same collumn as val */
    int downNeighbor = getLowerNeighbor(l, val);
    /*If there is no such neighbor return false */
    if (downNeighbor == 0 || downNeighbor == -1)
    {
        return false;
    }
    Bar *b = getBar(l, downNeighbor);

    /*If the neighbor is not part of the route then return false */
    if (b->vals[0] != route && b->vals[1] != route)
        return false;

    return true;
}

bool emptyCell(Ladder *l, int row, int col)
{
    if (l->ladder[row][col] == 0)
        return true;

    return false;
}

bool emptyRow(Ladder *l, int row)
{
    forall(l->numCols)
    {
        if (l->ladder[row][x] != 0)
            return false;
    }
    return true;
}

/*Gets the heigh of the ladder. Should be called get height, but
too lazy to change name. Just remember the variable depth in the ladder is
really its height */
int getDepth(Ladder *l)
{
    for (int i = MAXROWS - 1; i >= 0; i--)
    {
        if (emptyRow(l, i) == false)
            return i;
    }
    return MAXROWS;
}

/*
Checks if a value can be added at l->[row][col].

A value can be added to l->[row][col] iff


Case 1: col = 0 then a value can be added iff
l->[row][col] = 0 and l->[row][col+1] = 0

Case 2: col = l->numCols-1 then a value can be added iff
l->[row][col-1] = 0 and l->[row][col] = 0

Case 3: col > 0 and < l-numCols-1 then a value can be added iff
l->[row][col-1] = 0 and l->[row][col] = 0 and l->[row][col+1] = 0
 */
bool canBeAddedToRow(Ladder *l, int row, int col)
{
    if (row < 0 || col < 0)
        return false;

    int leftCol;
    int rightCol;

    /*Then set the leftCol to 0. Avoid writing more code this way */
    if (col == 0)
    {
        leftCol = 0;
    }
    /*Else set it to col-1 */
    else
    {
        leftCol = col - 1;
    }

    /*If the col is as far right as possible, then set the right to col in order
    to avoid writing more code */
    if (col == l->numCols - 1)
    {
        rightCol = col;
    }
    /*Else set it to col +1 */
    else
    {
        rightCol = col + 1;
    }

    /*If all three are empty then return true. Keep in mind that left or right could be = to col */
    if ((emptyCell(l, row, leftCol)) && (emptyCell(l, row, col)) && (emptyCell(l, row, rightCol)))
    {
        return true;
    }
    return false;
}

/**The following functions pertain to answering the following reasearch questions
 * Q1: Given a permutation, can you determine the number of ladders in its set of efficient ladder lotteries.
 * Hypothesis: The number of ladders in the set is equal to one more than the number of degenerative subsequences of size 3. 
 * A degenerative subequence of size 3 is defined as follows. Assume N is the largest value, N-X is the second largest 
 * value and N-Y is the third largest/smallest value in the degenerative subsequence. Assume X < Y. Then a degenerative subsequence means that N is to
 * the left of N-X which is to the left of N-Y in the permutation. If there are 2 degenerative subsequences then there are
 * three ladders in the set
 */

int countDegenerativeSubsequences(int *perm, int size)
{
    int count = 0;
    for (int i = 0; i < size - 2; i++)
    {
        for (int j = i + 1; j < size - 1; j++)
        {
            for (int k = j + 1; k < size; k++)
            {
                if (perm[i] > perm[j] && perm[j] > perm[k])
                    count++;
            }
        }
    }
    return count;
}

void displaySwapCount(Ladder *l)
{
    forall(l->numBars)
    {
        char *s = printBar(l->bars[x]);
        if(PRINT)
        print(s);
        clear(s);
    }
}

int factorial(int n)
{
    int result = 1;
    for (int i = n; i >= 1; i--)
    {
        result = result * i;
    }
    return result;
}

