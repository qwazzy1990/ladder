#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"

/*Global variables: used for clean level */

int MAXVAL;
int ladderCount = 1;
int I = 0;
char ladders[10000][10000];

bool DEBUG3 = true;

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
        clone->bars[x] = l->bars[x];
    }
    return clone;
}

Bar *newBar(void)
{
    new_object(Bar *, b, 1);
    b->set = true;
    return b;
}

/**DESTROYERS */
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
    if (size == 1)
    {
        for (int i = 1; i < l->numRows; i++)
        {
            for (int j = 0; j < l->numCols; j++)
            {
                int val = l->ladder[i][j];
                if (val != 0)
                {
                    int count = i - 1;
                    while (canBeAddedToRow(l, count, j) == true)
                    {
                        count--;
                    }
                    if (count < i - 1)
                    {
                        count++;
                        l->ladder[count][j] = val;
                        l->ladder[i][j] = 0;
                    }
                }
            }
        }
        l->depth = getDepth(l);
        l->cleanLevel = 1;
        return;
    }

    int largestIndex = getLargestIndex(perm, size);

    foreach (largestIndex, size)
    {
        //if an inversion is found
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
    copyArray(&arr, perm, largestIndex, size);
    createRoot(l, arr, size - 1, currRow);
    free(arr);
}

/**GETTERS */
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

int getFirstAvailableRow(Ladder *l, int currRow, int col)
{
    foreach (currRow, l->numRows)
    {
        if (canBeAddedToRow(l, x, col))
        {
            return x;
        }
    }
    return -1;
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
    sprintf(temp, "%d", b->vals[1]);
    strcat(s, temp);
    strcat(s, "\n");
    return s;
}

void printLadder(Ladder *l)
{
    //printf("\nTEST: %d\n", l->depth + 1);
    for (int i = 0; i <= l->depth+1; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val == 0)
                printf("[0 0] ");
            else
            {
                Bar *b = getBar(l, val);

                printf( "[%d %d] ", b->vals[0], b->vals[1]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

char *ladderToString(void *l)
{
    Ladder *ll = (Ladder *)l;
    char *s = calloc(100000, sizeof(char));
    int x = 0;
    for (int i = 0; i < ll->numRows; i++)
    {
        for (int j = 0; j < ll->numCols; j++)
        {
            char c = (char)ll->ladder[i][j] + 30;
            s[x] = c;
            x++;
        }
    }
    return s;
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

/**Gets the upper neighbor of the upper neigbor of the current value
 * Sets the rowToGo to the row of the upper neighbors, upper nieghbor+1
 */
int getRowToGo(Ladder *l, int val)
{

    int upNeighbor = getUpperNeighbor(l, val);
    int rowIndex = -1;
    //if there is no upper neighbor an error has occurred
    if (upNeighbor == -1)
        return rowIndex;

    else
    {
        rowIndex = getRowIndex(l, upNeighbor) - 1;
    }
    //get the upper neighbor of the upper neighbor
    upNeighbor = getUpperNeighbor(l, upNeighbor);

    //if there is no such neighbor, then the value goes one row above it's upper neighbor
    if (upNeighbor == -1)
    {

        while (canBeAddedToRow(l, rowIndex, getColIndex(l, val) + 1))
            rowIndex--;

        rowIndex++;
    }

    //else the value goes to the row below the upper neighbor of the value's upper neighbor
    else
    {

        rowIndex = getRowIndex(l, upNeighbor) + 1;
        while (canBeAddedToRow(l, rowIndex, getColIndex(l, val) + 1) == false)
        {
            rowIndex++;
        }
        rowIndex--;
    }

    return rowIndex;
}
void rightSwap(Ladder *l, int currRow, int currCol, int row, int col, int *mode)
{
    /*If it can't be swapped then return */
    if (isRightSwappable(l, l->ladder[currRow][currCol]) == false)
        return;

    //If the row is < 0 then you need to readjust the ladder accordingly

    /*Case 1 of right swap */
    if (row < 0 && canBeAddedToRow(l, 0, col) == true)
    {
        int offset = row * (-1);
        shiftLadderUp(l, (l->depth), 0, offset);
        row = 0;
        currRow++;
    } //end if
    //The general rule for swapping a bar to the right is that you want to swap it as high as it
    //Can go. The conditions are that the active bar cannot go higher than any bar in the upper right region.
    //Upper left corner of active region = the lowest bar in the collumn the active bar is going to that is
    //above the upper right neighbor of the active bar
    //Bottom left corner is the row and collumn with that has the upper right neighbor of the active bar.
    //upper right corner is top right of ladder
    //bottom left corner is the
    //clean route ends.
    //Any bar already in this region cannot be lower than the active bar going into this region

    //What to do: if space is open then put active bar there.
    //Else what happens is take the  right neighbor of active bar and swap places with it and active bar.
    //Then take the upper neighbor of active bar and make it the bottom right neighbor of the right neighbor
    //of the active bar
    //Then shift everything in the lower active region down by 2

    //get the upperNeigbor
    int upperNeighbor = getUpperNeighbor(l, l->ladder[currRow][currCol]);

    //Get the rightNeighbor
    int rightNeighbor = getRightNeighbor(l, l->ladder[currRow][currCol]);

    //get the row and collumn of the upper neighbor
    int upArr[2] = {-1, -1};
    upArr[0] = getRowIndex(l, upperNeighbor);
    upArr[1] = getColIndex(l, upperNeighbor);

    //get the row and collumn of the right neighbor
    int rightArr[2] = {-1, -1};
    rightArr[0] = getRowIndex(l, rightNeighbor);
    rightArr[1] = getColIndex(l, rightNeighbor);

    //if the value can be moved to the row and col
    /*Case 2 of right swap. Case 1 can only happen if case 2 is possible.
    Case 2: the value can be added to the row and col */
    if (canBeAddedToRow(l, row, col))
    {
        *mode = 0;
        //Check to see if there is another bar in the active region with the same route as the active bar
        //in the left collumn of where the active bar is going.
        //If so, continue to move the active bar up as long as it can and is to the bottom right of
        //its parent bar in the path OR there are cells above it that are empty
        //stop condition is if it hits the clean level

        //swap the upper neighbor and the right neighbor
        swapVals(&(l->ladder[upArr[0]][upArr[1]]), &(l->ladder[rightArr[0]][rightArr[1]]));
        if (emptyRow(l, upArr[0]))
        {
            readjustLadder(l, upArr[0] + 1, l->depth, -1);
        }
        if (emptyRow(l, rightArr[0]))
        {
            readjustLadder(l, rightArr[0] + 1, l->depth, -1);
        }

        int activeBar = l->ladder[currRow][currCol];

        /*Shift the acnestors of the current value by their respective offsets so the ladder is the correct height */

        int leftChild = getLeftChild(l, activeBar);
        int leftOffset = -1;

        l->ladder[row][col] = activeBar;
        l->ladder[currRow][currCol] = 0;
        if (emptyRow(l, currRow))
        {
            readjustLadder(l, currRow + 1, l->depth, -1);
        }

        /*If there is a left child of the current value then shift it and all its ancestors up
        by the correct offset for the left subtree */

        leftOffset = calculateChildOffset(l, leftChild);
        shiftChildren(l, leftChild, leftOffset);

    } //end if
    /*Case 3: The value cannot be added to the row and col */
    else
    {
        *mode = 1;
        //case 2: the cell to go to is blocked by clean level bar. If so then shift the bar down until there is
        //an open cell directly below.
        //Each time check if there is a bar directly to the right that is part of clean level k and
        //a bar directly to the left that is part of level k-1.
        //Each time check that row for a bar to the right that is not
        //part of the clean level.

        //If there is nothing to the left or right and there is no bar to the right not part of the clean level
        //then stop
        //else if there is a bar directly to the left and right then shift the left paths down by 2. then
        //shift the active bar down one more row
        //Remember to readjust the ladder for empty rows.

        //End point is the lower

        //Case to the cell to go to is open but there are unopen cells to the left and right.
        //In this case right bar is in the active region, left bar belongs to k-1 where k is the clean level
        //shift every bar <= k-1 down by 2

        /*Swap the right parent of the current value with the current value */
        int activeBar = l->ladder[currRow][currCol];
        swapVals(&(l->ladder[rightArr[0]][rightArr[1]]), &(l->ladder[currRow][currCol]));
        //swapVals(&(l->ladder[upArr[0]][upArr[1]]), &(l->ladder[currRow][currCol+1]));

        /* int lowerBound = currRow;
        while (emptyRow(l, lowerBound) == false)
        {
            lowerBound++;
        }
        lowerBound--;*/

        /*Create and shift reactangle such that it has a width defined by the currentRow + two rows and the row
        of the last value in the ladder. It's length is the number of collumns in the ladder
        Shift every value in this rectangle up the ladder by 2 */
        //shiftRectangle(l, currRow + 2, 0, lowerBound, l->numCols, 2);

        /*Create a reactangle such that its width is defined by the current row and the current row+1
        It's length is defined by starting at the current value's collumn +1 and ends atthe number of collumns in the ladder 
        Shift everything in this rectangle up by 2 */
        //shiftRectangle(l, currRow, currCol + 1, currRow + 1, l->numCols, 2);

        shiftChildrenDown(l, getRightChild(l, activeBar), 2);

        swapVals(&(l->ladder[upArr[0]][upArr[1]]), &(l->ladder[currRow + 1][currCol + 1]));
        if (emptyRow(l, upArr[0]))
        {
            int depth = getDepth(l);
            readjustLadder(l, upArr[0] + 1, depth, -1);
        }

    } //end else

    //l->depth = getDepth(l);
}

/*Reverse engineers right swap:
Must be adapted to the case in which rightSwap occurred. This will allow the */
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

int getLeftNeighbor(Ladder *l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);
    if (row <= 0)
        return -1;
    if (col <= 0 || col >= l->numCols - 1)
        return -1;

    row++;
    col--;
    for (int i = row; i <= l->depth; i++)
    {
        if (l->ladder[i][col] != 0)
            return l->ladder[i][col];
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

int getCleanLevel(Ladder *l)
{

    /*start at the end of the array and go backwards**/
    for (int i = 0; i < l->depth; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val != 0)
            {
                Bar *b1 = getBar(l, val);
                if (b1 == NULL)
                    continue;
                int upperNeighbor = getUpperNeighbor(l, val);
                Bar *b2 = getBar(l, upperNeighbor);
                if (b2 == NULL)
                    continue;
                if (b2->routeNum < b1->routeNum)
                {
                    return b1->routeNum + 1;
                }
            }
        }
    }
    return 1;
}

void readjustLadder(Ladder *l, int start, int end, int offset)
{
    for (int i = start; i <= end; i++)
    {
        shiftLadderDown(l, i + offset, i);
        makeRowEmpty(l, i);
    }
    l->depth = getDepth(l);
}
void shiftLadderDown(Ladder *l, int dest, int source)
{
    forall(l->numCols)
    {
        l->ladder[dest][x] = l->ladder[source][x];
    }
}

void shiftLadderUp(Ladder *l, int start, int end, int offset)
{
    for (int i = start; i >= end; i--)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            l->ladder[i + offset][j] = l->ladder[i][j];
        }
        makeRowEmpty(l, i);
    }
}

void fixLadder(Ladder *l)
{
    int end = l->depth+10;
    for (int i = 1; i <= end; i++)
    {
        for (int j = l->numCols - 1; j >= 0; j--)
        {
            if (l->ladder[i][j] != 0)
            {
                int val = l->ladder[i][j];
                if (ladderCount == 15)
                {
                    Bar *b = getBar(l, val);
                    char *s = printBar(b);
                    print(s);
                    clear(s);
                }

                int temp = getRowIndex(l, val) - 1;

                while (canBeAddedToRow(l, temp, j))
                {
                    temp--;
                }
                temp++;
                l->ladder[i][j] = 0;

                l->ladder[temp][j] = val;
            }
        }
        l->depth = getDepth(l);
    }

    l->depth = getDepth(l);
    
}

//val is the child to be shifted
void shiftChildren(Ladder *l, int val, int offset)
{
    //if there were no more children
    if (val == 0 || val == -1)
        return;
    if (offset == 0)
        return;

    //get the row and col index of the child
    int rowIndex = getRowIndex(l, val);
    int colIndex = getColIndex(l, val);

    //calculate how many rows the child can be shifted

    //put the child in the right spot in the ladder
    l->ladder[rowIndex + offset][colIndex] = val;
    l->ladder[rowIndex][colIndex] = 0;

    if (colIndex < l->numCols - 1)
    {
        int rightChild = l->ladder[rowIndex + 1][colIndex + 1];
        offset = calculateChildOffset(l, rightChild);

        shiftChildren(l, rightChild, offset);
    }
    //get the left child of the child and shift it
    if (colIndex > 0)
    {
        int leftChild = l->ladder[rowIndex + 1][colIndex - 1];
        offset = calculateChildOffset(l, leftChild);

        shiftChildren(l, leftChild, offset);
    }

    //get the right child of the child and shitf it
}

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

int calculateChildOffset(Ladder *l, int leftChild)
{
    if (leftChild == -1 || leftChild == 0)
        return -1;

    int rowIndex = getRowIndex(l, leftChild);
    int colIndex = getColIndex(l, leftChild);

    rowIndex--;
    int offset = 0;
    while (canBeAddedToRow(l, rowIndex, colIndex))
    {
        offset--;
        rowIndex--;
    }
    return offset;
}

void makeRowEmpty(Ladder *l, int row)
{
    forall(l->numCols)
    {
        l->ladder[row][x] = 0;
    }
}
void driver(int *perm, int size)
{
    //create a ladder
    Ladder *l = newLadder(size);

    //initialize it
    initLadder(l);

    createRoot(l, perm, size, 0);
    printLadder(l);
    /* if(DEBUG3)
    {
        fixLadder(l);
    }*/

    qsort(perm, size, sizeof(int), compareInts);
    MAXVAL = perm[size - 1];

    findAllChildren(l, 1, 0);
    destroyLadder(l);

    //call find all children with clean level = 1
}

void findAllChildren(Ladder *l, int cleanLevel, int level)
{
    if (l == NULL)
        return;

    fixLadder(l);
    char *s = ladderToString(l);
    //strcpy(ladders[I], s);
    for (int i = 0; i < I; i++)
    {
        if (strcmp(ladders[i], s) == 0)
        {
            clear(s);
            return;
        }
    }
    strcpy(ladders[I], s);
    clear(s);

    I++;

    //printf("Clean level %d\n", cleanLevel);
    //printf("\nLADDER NUMBER: %d", ladderCount);
    printf( "Clean Level:%d\n" , cleanLevel);
    printf( "Level %d\n" , level);

    printf( "Ladder Number:%d\n", ladderCount);
    printLadder(l);
    ladderCount++;

    Ladder *clone = cloneLadder(l);

    int y = MAXVAL;
    while (y >= cleanLevel)
    {
        int start[2] = {-1, -1};
        int end[2] = {-1, -1};
        getStartOfRoute(l, y, start);
        getEndOfRoute(l, y, end);
        if (start[0] != -1)
            for (int i = start[0]; i <= end[0]; i++)
            {
                for (int j = start[1]; j <= end[1]; j++)
                {
                    int val = l->ladder[i][j];
                    if (val == 0)
                        continue;
                    Bar *b = getBar(l, val);
                    if (b->routeNum == y)
                    {
                        int lowerNeighbor = getLowerNeighbor(l, val);
                        if (isRightSwappable(l, lowerNeighbor))
                        {
                            int mode = 0;
                            rightSwap(l, getRowIndex(l, lowerNeighbor), getColIndex(l, lowerNeighbor), getRowToGo(l, lowerNeighbor), getColIndex(l, lowerNeighbor) + 1, &mode);
                            findAllChildren(l, b->routeNum + 1, level + 1);
                            leftSwap(l, clone);
                            //destroyClone(clone);
                        }
                    }
                }
            }
        y--;
    }

    int start[2] = {-1, -1};
    int end[2] = {-1, -1};
    getStartOfRoute(l, cleanLevel - 1, start);
    getEndOfRoute(l, cleanLevel - 1, end);
    if (start[0] != -1)
        for (int i = start[0]; i <= end[0]; i++)
        {
            for (int j = start[1]; j <= end[1]; j++)
            {
                int val = l->ladder[i][j];
                if (val == 0)
                    continue;
                Bar *b = getBar(l, val);
                if (b->routeNum == y)
                {
                    int lowerNeighbor = getLowerNeighbor(l, val);
                    if (isRightSwappable(l, lowerNeighbor) && canBeActiveBar(l, lowerNeighbor, y))
                    {
                        int mode = 0;
                        rightSwap(l, getRowIndex(l, lowerNeighbor), getColIndex(l, lowerNeighbor), getRowToGo(l, lowerNeighbor), getColIndex(l, lowerNeighbor) + 1, &mode);
                        findAllChildren(l, cleanLevel, level + 1);
                        leftSwap(l, clone);
                        //destroyClone(clone);
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

//Gets the firts turn bar, which is
//The value that is highest on the ladder with the property of being
//right swappable. If there is no such value then the root
//is the only ladder generated by the permutation. The entire ladder is monotone and the algorithm
//will returns
int getFirstTurnBar(Ladder *root)
{
    for (int i = 0; i < root->depth; i++)
    {
        for (int j = 0; j < root->numCols; j++)
        {
            if (isRightSwappable(root, root->ladder[i][j]))
            {
                return root->ladder[i][j];
            }
        }
    }
    //if there is no firts turn bar. The root is monotone
    return -1;
}

int getStartOfRoute(Ladder *l, int routeNum, int *arr)
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
                    return 0;
                }
            }
        }
    }
    return -1;
}

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

bool canBeActiveBar(Ladder *l, int val, int k)
{
    //Clone the ladder to not mess up the original ladder
    Ladder *clone = cloneLadder(l);

    int mode = 0;
    //rightswap the value in the clone
    rightSwap(clone, getRowIndex(clone, val), getColIndex(clone, val), getRowToGo(clone, val), getColIndex(clone, val) + 1, &mode);

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

bool isUpwardVisible(Ladder *l, int val, int route)
{
    if (val == -1 || val == 0)
        return false;
    int rowIndex = getRowIndex(l, val);
    int colIndex = getColIndex(l, val);

    int left;
    int right;

    if (colIndex != 0)
    {
        left = colIndex - 1;
    }
    else
    {
        left = -1;
    }
    if (colIndex != l->numCols - 1)
    {
        right = colIndex + 1;
    }
    else
    {
        right = -1;
    }

    if (left != -1)
    {
        for (int i = rowIndex; i < l->depth; i++)
        {
            int leftVal = l->ladder[i][left];
            if (leftVal != 0)
            {
                Bar *b = getBar(l, leftVal);
                if (b->vals[0] != route && b->vals[1] != route)
                    return false;
                else
                    break;
            }
        }
    }

    if (right != -1)
    {
        for (int i = rowIndex; i < l->depth; i++)
        {
            int rightVal = l->ladder[i][right];
            if (rightVal != 0)
            {
                Bar *b = getBar(l, rightVal);
                if (b->vals[0] != route && b->vals[1] != route)
                    return false;
                else
                    break;
            }
        }
    }

    int downNeighbor = getLowerNeighbor(l, val);
    if (downNeighbor == 0 || downNeighbor == -1)
    {
        return false;
    }
    Bar *b = getBar(l, downNeighbor);

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

int getDepth(Ladder *l)
{
    for (int i = MAXROWS - 1; i >= 0; i--)
    {
        if (emptyRow(l, i) == false)
            return i;
    }
    return MAXROWS;
}

bool canBeAddedToRow(Ladder *l, int row, int col)
{
    if (row < 0 || col < 0)
        return false;
    int leftCol;
    int rightCol;
    if (col == 0)
    {
        leftCol = 0;
    }
    else
    {
        leftCol = col - 1;
    }

    if (col == l->numCols - 1)
    {
        rightCol = col;
    }
    else
    {
        rightCol = col + 1;
    }

    if ((emptyCell(l, row, leftCol)) && (emptyCell(l, row, col)) && (emptyCell(l, row, rightCol)))
    {
        return true;
    }
    return false;
}
