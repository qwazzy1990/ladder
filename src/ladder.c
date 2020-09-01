#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"
#include "LinkedListAPI.h"
#include <ctype.h>

/*Global variables: used for clean level */

int MAXVAL;
int ladderCount = 1;

bool DEBUG3 = true;

bool MIN = false;

int CURMIN = 1000000;

List *minLadders;

void printAllPerms(List *perms, int n)
{
    PRINT = true;
    printf("---NUMBER OF ITEMS: %d---\n", perms->length);
    for (Node *h = perms->head; h != NULL; h = h->next)
    {
        printPerm(h->data, n);
    }
    PRINT = false;
}

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
    l->ladderNumber = -1;
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
    clone->ladderNumber = l->ladderNumber;

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

Bar *cloneBar(Bar *b)
{
    new_object(Bar *, bb, 1);
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
    forall(100) destroyBar(ll->bars[x]);
    clear(ll->bars);
    clear(ll);
}

void destroyDTSA(void *dts)
{
    DTSA *d = dts;
    forall(d->amnt)
    {
        free(d->dts[x]);
    }
    free(d->dts);
    free(d);
}
/*END DESTROYERS */

void initLadder(Ladder *l)
{
    l->ladder = calloc(MAXROWS, sizeof(int *));

    //Create the ladder
    forall(l->numRows)
    {
        l->ladder[x] = calloc(l->numCols, sizeof(int));
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
    if (l == NULL || perm == NULL)
        return;
    if (size == 1)
    {
        /*Fix the ladder by shifting every value as high as it can go */
        if (l->numBars == 0)
        {
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

void minLadderDriver(int *perm, int n)
{
    Ladder *l = newLadder(n);
    initLadder(l);
    int *tempPerm = copyIntArr(perm, n);
    //run the first algo
    printf("---RUNNING FIRST ALGO---\n");

    new_object(bool *, toBeFlipped, n);
    new_object(bool *, beenFlipped, n);
    forall(n)
    {
        toBeFlipped[x] = false;
        beenFlipped[x] = false;
    }
    createMinLadder(l, tempPerm, n, 0);
    printLadder(l);
    destroyLadder(l);
    free(tempPerm);
    free(toBeFlipped);
    free(beenFlipped);
}
int funcGetCount(int *perm, int start, int *end, int n)
{
    int count = 1;
    while (perm[start] > perm[start + 1])
    {

        start++;
        count++;
        if (start == n - 1)
            break;
    }
    *end = start;
    return count;
}

void swapDSS(Ladder *l, int *perm, int start, int count, int currRow)
{

    for (int i = 0; i < count / 2; i++)
    {
        addBar(l, perm[start], perm[start + 1], currRow, start);

        swapInts(&(perm[start]), &(perm[start + 1]));
        start += 2;
    }
}

int *copyA(int *src, int start, int end)
{
    int *cpy = calloc((end - start) + 1, sizeof(int));
    int c = 0;
    for (int i = start; i <= end; i++)
    {
        cpy[c] = src[i];
        c++;
    }
    return cpy;
}
void preProcessRowZero(Ladder *l, int *perm, int n)
{
    int start;
    int end;
    int count;
    forall(n - 1)
    {
        //if a DSS was found
        if (perm[x] > perm[x + 1])
        {
            count = funcGetCount(perm, x, &end, n);
            //if count is even, then just swap
            if (!(count % 2))
            {
                swapDSS(l, perm, x, count, 0);
            }
            //else try swapping the first two and see if a DSS3 or greater is created to the left.
            else
            {
                swapInts(&(perm[x]), &perm[x + 1]);
                int *cpy = copyA(perm, 0, x + 1);
                int numDSS = countDegenerativeSubsequences(cpy, x + 1);
                //if swapping the first 2 created no DSS3 or greater then we are good
                if (numDSS == 0)
                {
                    addBar(l, perm[x + 1], perm[x], 0, x);
                    swapDSS(l, perm, x + 2, count - 2, 0);
                }
                //swap it back and swap the other two
                else
                {
                    swapInts(&(perm[x]), &perm[x + 1]);
                    swapInts(&(perm[x + 1]), &(perm[x + 2]));
                    addBar(l, perm[x + 2], perm[x + 1], 0, x + 1);
                    swapDSS(l, perm, x + 3, count - 3, 0);
                }
            }
            x = end;
        }
    }
}

void createMinLadder(Ladder *l, int *perm, int n, int currRow)
{
    printPerm(perm, n);

    if (currRow == 0)
    {
        preProcessRowZero(l, perm, n);
        createMinLadder(l, perm, n, currRow + 1);
    }

    if (isSorted(perm, n))
        return;

    int end;
    int count;
    forall(n - 1)
    {
        if (perm[x] > perm[x + 1])
        {
            count = funcGetCount(perm, x, &end, n);
            if (!(count % 2))
            {
                swapDSS(l, perm, x, count, currRow);
            }
            else
            {
                addBar(l, perm[x + 1], perm[x + 2], currRow, x + 1);
                swapInts(&(perm[x + 1]), &(perm[x + 2]));
                swapDSS(l, perm, x + 3, count - 3, currRow);
            }
            x = end;
        }
    }
    createMinLadder(l, perm, n, currRow + 1);
    l->depth++;

    //     if (perm[x] < perm[x - 1])
    //     {
    //         Bar *b = newBar();
    //         setBar(b, l->numBars + 1, perm[x - 1], perm[x]);
    //         l->bars[l->numBars] = b;
    //         l->numBars++;
    //         l->ladder[currRow][x - 1] = b->barNum;
    //         swapInts(&(perm[x - 1]), &(perm[x]));
    //         x--;
    //     }
    // }
    // l->depth++;
    // int row = currRow + 1;

    //createMinLadder(l, perm, n, row);
}

void createMinLadderTwo(Ladder *l, int *perm, int n, int currRow)
{
    printPerm(perm, n);

    if (isSorted(perm, n))
        return;

    for (int x = 0; x < n - 1; x++)
    {
        if (perm[x] > perm[x + 1])
        {
            Bar *b = newBar();
            setBar(b, l->numBars + 1, perm[x], perm[x + 1]);
            l->bars[l->numBars] = b;
            l->numBars++;
            l->ladder[currRow][x] = b->barNum;
            swapInts(&(perm[x + 1]), &(perm[x]));
            x++;
        }
    }
    l->depth++;
    int row = currRow + 1;

    createMinLadderTwo(l, perm, n, row);
}

void createMinLadderThree(Ladder *l, int *perm, int n, int currRow, bool direction)
{
    printPerm(perm, n);

    if (isSorted(perm, n))
        return;
    if (direction)
    {
        for (int x = n - 1; x > 0; x--)
        {
            if (perm[x] < perm[x - 1])
            {
                Bar *b = newBar();
                setBar(b, l->numBars + 1, perm[x - 1], perm[x]);
                l->bars[l->numBars] = b;
                l->numBars++;
                l->ladder[currRow][x - 1] = b->barNum;
                swapInts(&(perm[x - 1]), &(perm[x]));
                x--;
            }
        }
    }
    else
    {
        for (int x = 0; x < n - 1; x++)
        {
            if (perm[x] > perm[x + 1])
            {
                Bar *b = newBar();
                setBar(b, l->numBars + 1, perm[x], perm[x + 1]);
                l->bars[l->numBars] = b;
                l->numBars++;
                l->ladder[currRow][x] = b->barNum;
                swapInts(&(perm[x + 1]), &(perm[x]));
                x++;
            }
        }
    }
    l->depth++;
    int row = currRow + 1;

    createMinLadderThree(l, perm, n, row, !direction);
}

bool isSorted(int *perm, int n)
{
    forall(n)
    {
        if (perm[x] != x + 1)
            return false;
    }
    return true;
}

int getTempRow(Ladder *l, int row, int col)
{
    int tempRow = row;
    if (canBeAddedToRow(l, row, col))
        return row;
    while (canBeAddedToRow(l, tempRow, col) == false)
    {
        tempRow++;
        if (l->ladder[tempRow][col - 1] != 0 || l->ladder[tempRow + 1][col] != 0)
            break;
    }
    return tempRow;
}
void fixLadderTwo(Ladder *l)
{
    for (int i = 0; i < MAXROWS; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            if (l->ladder[i][j] != 0)
            {
                int tempRow = i - 1;
                while (canBeAddedToRow(l, tempRow, j) && tempRow > 0)
                {
                    tempRow--;
                }
                tempRow++;
                //if it can be moved up the ladder
                if (tempRow < i)
                {
                    l->ladder[tempRow][j] = l->ladder[i][j];
                    l->ladder[i][j] = 0;
                }
            }
        }
    }
}

int getIndex(int *perm, int n, int len)
{
    forall(len)
    {
        if (perm[x] == n)
            return x;
    }
    return -1;
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
    PRINT = true;

    printf(CYAN "Ladder Number:%d\n" COLOR_RESET, l->ladderNumber);
    printf(MAGENTA "Height:%d\n" COLOR_RESET, l->depth + 1);
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
    PRINT = false;
}

void printAllLadders(List *ladders)
{
    PRINT = true;
    printf("---NUMBER OF ITEMS: %d---\n", ladders->length);
    for (Node *h = ladders->head; h != NULL; h = h->next)
    {
        printLadder((Ladder *)h->data);
    }
    PRINT = false;
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

void addBar(Ladder *l, int valOne, int valTwo, int row, int col)
{
    Bar *b = newBar();
    setBar(b, l->numBars + 1, valOne, valTwo);
    l->ladder[row][col] = b->barNum;
    l->bars[l->numBars] = b;
    l->numBars++;
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

    // fixLadder(l);
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

void leftSwapTwo(Ladder *l, int currBar, int leftNeighbor, int botNeighbor)
{

    //row and col for left neighbor
    int rLeft = getRowIndex(l, leftNeighbor);
    int cLeft = getColIndex(l, leftNeighbor);
    //row and col for bot neighbor
    int rBot = getRowIndex(l, botNeighbor);
    int cBot = getColIndex(l, botNeighbor);

    //swap left neighbor with bot neighbor
    swapVals(&(l->ladder[rLeft][cLeft]), &(l->ladder[rBot][cBot]));

    int rowToGo = rBot + 1;
    int colToGo = getColIndex(l, currBar) - 1;
    int rCurr = getRowIndex(l, currBar);
    swapVals(&(l->ladder[rCurr][colToGo + 1]), &(l->ladder[rowToGo][colToGo]));

    //call the function to readjust the ladder
    Bar *bb = getBar(l, botNeighbor);
    int routeCrossed = bb->routeNum;
    fixCleanLevel(l, currBar, routeCrossed);
    fixLadder(l);
}

void fixCleanLevel(Ladder *l, int barNum, int routeCrossed)
{
    if (allBelow(l, barNum, routeCrossed))
        return;

    int colIndex = getColIndex(l, barNum);
    int rowIndex = getRowIndex(l, barNum);
    Bar *theBar = getBar(l, barNum);
    for (int i = 0; i <= l->depth + 1; i++)
        for (int j = 0; j < colIndex; j++)
        {
            if (l->ladder[i][j] != 0)
            {
                //get route number
                //get row
                //if row is > bar's row shift the bar down by 1
                //
                Bar *b = getBar(l, l->ladder[i][j]);
                int rIndex = getRowIndex(l, l->ladder[i][j]);
                int cIndex = getColIndex(l, l->ladder[i][j]);

                if (b->routeNum < theBar->routeNum && rIndex <= rowIndex && belowLevel(l, b->barNum, routeCrossed))
                {
                    swapVals(&(l->ladder[rIndex][cIndex]), &(l->ladder[rIndex + 1][cIndex]));
                }
            }
        }

    fixCleanLevel(l, barNum, routeCrossed);
}

bool allBelow(Ladder *l, int barNum, int routeCrossed)
{
    int colIndex = getColIndex(l, barNum);
    int rowIndex = getRowIndex(l, barNum);
    Bar *theBar = getBar(l, barNum);
    for (int i = 0; i <= l->depth; i++)
    {
        for (int j = 0; j < colIndex; j++)
        {
            if (l->ladder[i][j] != 0)
            {
                //
                Bar *b = getBar(l, l->ladder[i][j]);
                int rIndex = getRowIndex(l, l->ladder[i][j]);
                bool below = belowLevel(l, b->barNum, routeCrossed);

                if (rIndex < rowIndex && b->routeNum < theBar->routeNum && below)
                    return false;
            }
        }
    }
    return true;
}

bool belowLevel(Ladder *l, int barNum, int level)
{
    int rowNum = getRowIndex(l, barNum);
    int colIndex = getColIndex(l, barNum);
    for (int i = rowNum - 1; i >= 0; i--)
    {
        if (l->ladder[i][colIndex] != 0)
        {
            Bar *b = getBar(l, l->ladder[i][colIndex]);
            if (b->routeNum == level)
                return true;
        }
    }
    return false;
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
                return row;
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
                return col;
            }
        }
    }
    return col;
}

//Gets the upper neighbor of value n where the upper
//neighbor, v, is defined as a value > 0 in the ladder with
//v's collumn value = to n's collumn value/and v's row value x, where 0 <= x < n's row value
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

int getLeftNeighbor(Ladder *l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);
    if (row >= l->depth)
        return -1;
    if (col <= 0)
        return -1;
    for (int i = row + 1; i <= l->depth; i++)
    {
        if (l->ladder[i][col - 1] != 0)
            return l->ladder[i][col - 1];
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

void driver(int *perm, int size)
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
    // printf("---Min Ladder is ----\n");
    // printLadder(l);
    // printf("----Done Min Ladder----\n");
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

    new_object(DTSA *, dts, 1);
    //setDTSs(dts, perm, countDegenerativeSubsequences(perm, size), size);

    //Call find all children with clean level 1 because l is currently the root.
    //The only ladder in the set with a clean level of 1.
    findAllChildren(l, 1, 0, dts);
    // if (PRINT)
    //     printf("The number of degenerative subsequences is %d\n", countDegenerativeSubsequences(perm, size));
    //displaySwapCount(l);

    destroyLadder(l);

    destroyDTSA(dts);
    ladderCount = 1;
}

void findAllChildren(Ladder *l, int cleanLevel, int level, DTSA *dts)
{
    if (l == NULL)
        return;
    if (PRINT)
        printDtsAsBinString(dts);
    l->ladderNumber = ladderCount;
    if (MIN)
    {
        if (l->depth < CURMIN)
        {
            //clone the ladder
            Ladder *clone = cloneLadder(l);
            clone->ladderNumber = ladderCount;
            //clear the list
            clearList(minLadders);
            insertBack(minLadders, clone);
            //add it to the list
            CURMIN = clone->depth;
            //set CURMIN to l->depth
        }
        else if (l->depth == CURMIN)
        {
            Ladder *clone = cloneLadder(l);
            clone->ladderNumber = ladderCount;
            insertBack(minLadders, clone);
        }
    }
    if (SAVEALL)
    {
        Ladder *clone = cloneLadder(l);
        //clone->ladderNumber = ladderCount;
        insertBack(allLadders, clone);
    }
    if (PRINT)
    {
        printf(RED "Clean Level:%d\n" COLOR_RESET, cleanLevel);
        printf(YELLOW "Depth:%d\n" COLOR_RESET, level);
        printLadder(l);
    }

    //if the ladder has no bars, then there is only one, so return
    if (l->numBars == 0)
        return;

    ladderCount++;
    Ladder *clone = cloneLadder(l);

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
            for (int i = end[0]; i >= start[0]; i--)
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
                            int up = getUpperNeighbor(l, bb->barNum);
                            int right = getRightNeighbor(l, bb->barNum);
                            Bar *upBar = getBar(l, up);
                            Bar *rightBar = getBar(l, right);

                            /*Swap the bar*/
                            rightSwap(l, lowerNeighbor);
                            rotateDTS(dts, bb->vals[0], bb->vals[1], rightBar->vals[0], rightBar->vals[1], upBar->vals[0], upBar->vals[1], dts->amnt);

                            //clear(bars);

                            /*Recursive call with clean level = to b->routeNum+1*/
                            findAllChildren(l, b->routeNum + 1, level + 1, dts);
                            rotateDTS(dts, bb->vals[0], bb->vals[1], rightBar->vals[0], rightBar->vals[1], upBar->vals[0], upBar->vals[1], dts->amnt);
                            //leftSwapTwo(l, lowerNeighbor, getLeftNeighbor(l, lowerNeighbor), getLowerNeighbor(l, lowerNeighbor));

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
        for (int i = end[0]; i >= start[0]; i--)
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
                        int up = getUpperNeighbor(l, bb->barNum);
                        int right = getRightNeighbor(l, bb->barNum);
                        Bar *upBar = getBar(l, up);
                        Bar *rightBar = getBar(l, right);

                        rightSwap(l, lowerNeighbor);
                        rotateDTS(dts, bb->vals[0], bb->vals[1], rightBar->vals[0], rightBar->vals[1], upBar->vals[0], upBar->vals[1], dts->amnt);

                        findAllChildren(l, cleanLevel, level + 1, dts);
                        rotateDTS(dts, bb->vals[0], bb->vals[1], rightBar->vals[0], rightBar->vals[1], upBar->vals[0], upBar->vals[1], dts->amnt);
                        //leftSwapTwo(l, lowerNeighbor, getLeftNeighbor(l, lowerNeighbor), getLowerNeighbor(l, lowerNeighbor));

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
        if (PRINT)
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

//Functions for Degenerative Triadic Subsequences

void setDTSs(DTSA *dts, int *perm, int numDts, int size)
{
    dts->dts = calloc(numDts, sizeof(DTS *));
    dts->amnt = numDts;
    forall(numDts)
    {
        dts->dts[x] = calloc(1, sizeof(DTS));
        dts->dts[x]->num = 0;
        dts->dts[x]->isRotated = false;
    }
    int count = 0;
    int x = 0;

    for (int i = 0; i < size - 2; i++)
    {
        for (int j = i + 1; j < size - 1; j++)
        {
            for (int k = j + 1; k < size; k++)
            {
                if (perm[i] > perm[j] && perm[j] > perm[k])
                {
                    dts->dts[x]->top[0] = perm[i];
                    dts->dts[x]->top[1] = perm[j];
                    dts->dts[x]->mid[0] = perm[i];
                    dts->dts[x]->mid[1] = perm[k];
                    dts->dts[x]->lower[0] = perm[j];
                    dts->dts[x]->lower[1] = perm[k];
                    dts->dts[x]->isRotated = false;
                    dts->dts[x]->num = count + 1;
                    count++;
                    if (PRINT)
                        printDTS(dts->dts[x]);
                    x++;
                }
            }
        }
    }
}
void rotateDTS(DTSA *dts, int l1, int l2, int m1, int m2, int t1, int t2, int numDts)
{

    forall(numDts)
    {
        if (dts->dts[x]->lower[0] == l1 && dts->dts[x]->lower[1] == l2 && dts->dts[x]->mid[0] == m1 && dts->dts[x]->mid[1] == m2 && dts->dts[x]->top[0] == t1 && dts->dts[x]->top[1] == t2)
        {
            dts->dts[x]->isRotated = !(dts->dts[x]->isRotated);
        }
    }
}
void printDTSA(DTSA *dts)
{

    printf("----Inside print DTSA----Amount is %d----\n", dts->amnt);
    forall(dts->amnt)
    {
        printDTS(dts->dts[x]);
    }
}

void printDTS(DTS *d)
{
    printf("T1 %d T2 %d M1 %d M2 %d L1 %d L2 %d Number %d Rotated %d\n", d->top[0], d->top[1], d->mid[0], d->mid[1], d->lower[0], d->lower[1], d->num, d->isRotated);
}

void printDtsAsBinString(DTSA *dts)
{
    if (ladderCount == 1)
        forall(dts->amnt)
        {
            printf("[%d,%d,%d]  ", dts->dts[x]->top[0], dts->dts[x]->top[1], dts->dts[x]->lower[1]);
        }
    printf("\n");
    forall(dts->amnt)
    {
        if (dts->dts[x]->isRotated)
        {
            printf(GREEN "   1     " COLOR_RESET);
        }
        else
        {
            printf("   0     ");
        }
    }
    printf("%d\n", ladderCount);
    printf("\n");
}

void freeDts(DTSA *dts, int n)
{
    forall(n)
    {
        free(dts->dts[x]);
    }
    free(dts->dts);
    free(dts);
}

void genMinLadders(int *perm, int numDig)
{
    minLadders = initializeList(dummy_print, destroyLadder, dummy_compare);
    MIN = true;
    driver(perm, numDig);
    for (Node *n = minLadders->head; n != NULL; n = n->next)
    {
        Ladder *minLadder = n->data;
        printf(CYAN "Ladder Number:%d\n" COLOR_RESET, minLadder->ladderNumber);
        printf(MAGENTA "Height:%d\n" COLOR_RESET, minLadder->depth + 1);
        printLadder(minLadder);
    }
    freeList(minLadders);

    MIN = false;
    CURMIN = 1000000;
}

/****ENCODING LADDERS SECTION****/

void saveAllLadders(int *perm, int numDig)
{
    allLadders = initializeList(dummy_print, destroyClone, dummy_compare);
    SAVEALL = true;
    driver(perm, numDig);
    for (Node *h = allLadders->head; h != NULL; h = h->next)
    {
        Ladder *l = h->data;
        printLadder(l);
    }
    freeList(allLadders);
    SAVEALL = false;
}

void encodingDriver(int *perm, int numDig, int mode)
{
    saveAllLadders(perm, numDig);
    srand(time(NULL));
    int r = rand();
    r = r % allLadders->length;
    r = 10;
    Ladder *encode = (Ladder *)allLadders->head->data;
    Node *temp = allLadders->head;

    forall(r)
    {

        if (x == r - 1)
        {
            encode = temp->data;
            break;
        }
        temp = temp->next;
    }
    printLadder(encode);
    if (mode == 0)
    {
        ilEncode(encode);
        //gen a random number b/w 0 and len(allladders)
    } //call the ilencoding

    else
    {
        printf("%d\n", adjustHeight(encode, 1, 2, 0));
        printLadder(encode);
        adjustHeight(encode, 0, 1, 1);
        printLadder(encode);
    } //call height encoding
    freeList(allLadders);
}

char *ilEncode(Ladder *l)
{
    printLadder(l);
    new_object(char *, final, 100);
    new_object(char *, first, l->depth + 3);
    new_object(char *, second, l->depth + 3);
    // bool flag = false;
    forall(l->depth + 2)
    {
        first[x] = 'x';
        second[x] = 'x';
    }

    for (int i = 0; i < l->numCols; i++)
    {
        for (int j = 0; j <= l->depth; j++)
        {
            if (i == 0 && l->ladder[j][i] > 0)
            {
                printf("%d\n", l->ladder[j][i]);
                first[j] = '1';
                second[j] = '0';
            }
        }
        first[l->depth + 1] = '0';
        first[l->depth + 2] = '\0';
        if (i == 0)
        {
            printf("%s\n", first);
        }
    }
    return NULL;
}

//For height based encoding

bool adjustHeight(Ladder *l, int startRow, int row, int col)
{
    if (l->ladder[row][col] != 0 || row > l->depth)
        return false;
    int leftCol = col - 2;
    int rightCol = col + 2;
    if (leftCol >= 0 && l->ladder[row][leftCol] != 0)
    {
        int temp = l->ladder[row][col];
        l->ladder[row][col] = l->ladder[row][leftCol];
        l->ladder[row][leftCol] = temp;
        return true;
    }
    if (rightCol < l->numCols && l->ladder[row][rightCol] != 0)
    {
        int temp = l->ladder[row][col];
        l->ladder[row][col] = l->ladder[startRow][col];
        l->ladder[startRow][col] = temp;
        return true;
    }
    return adjustHeight(l, startRow, row + 1, col);
}

List *copyListOfLadders(List *ogList)
{
    List *newList = initializeList(ogList->printData, ogList->deleteData, ogList->compare);
    for (Node *temp = ogList->head; temp != NULL; temp = temp->next)
    {
        insertBack(newList, cloneLadder((Ladder *)temp->data));
    }
    return newList;
}