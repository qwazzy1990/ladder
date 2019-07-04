#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"

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

Bar* newBar( void )
{
    new_object(Bar*, b, 1);
    b->set = true;
    return b;
}

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
    l->bars = calloc(100, sizeof(Bar*));
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
            Bar* b = newBar();

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

Bar* getBar(Ladder* l, int n)
{
    forall(l->numBars)
    {
        Bar* b = l->bars[x];
        if(b->barNum == n)
        {
            return b;
        }
    }
    return NULL;
}

/*Printers */
char *printBar(Bar* b)
{
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
    strcat(s, "\n\n");
    return s;
}

void printLadder(Ladder *l)
{
    printf("\nDEPTH: %d\n\n", l->depth);
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val == 0)
                printf("%d ", val);
            else
            {
                printf(GREEN "%c " COLOR_RESET, (char)(val + 96));
            }
        }
        printf("\n");
    }

    /* forall(l->numBars)
    {
        char *s = printBar(l->bars[x]);
        printf("%s", s);
        free(s);
    }*/
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

void rightSwap(Ladder *l, int currRow, int currCol, int row, int col)
{
    if (canBeAddedToRow(l, row, col))
    {
        int val = l->ladder[currRow][currCol];
        l->ladder[row][col] = val;
        l->ladder[currRow][currCol] = 0;
    }
    else
    {
        int x = l->depth;
        int val = l->ladder[currRow][currCol];

        int upperNeighbor = getUpperNeighbor(l, val);
        int rightNeighbor = getRightNeighbor(l, val);
        Bar* bUp = getBar(l, upperNeighbor);
        Bar* bRight = getBar(l, rightNeighbor);

        

        for (int i = x - 1; i > row; i--)
        {
            shiftLadderDown(l, x, i);
            makeRowEmpty(l, i);
            x--;
        }
        row++;
        currRow++;
        l->ladder[row][col] = val;
        l->ladder[currRow][currCol] = 0;
        /* do the shifting and emptying*/
    }

    l->depth = getDepth(l);
}


int getRowIndex(Ladder* l, int n)
{
    int row = -1;
    for(int i = 0; i < l->numRows; i++)
    {
        for(int j = 0; j < l->numCols; j++)
        {
            if(l->ladder[i][j] == n)
            {
                row = i;
            }
        }
    }
    return row;
}

int getColIndex(Ladder* l, int n)
{
    int col = -1;
    for(int i = 0; i < l->numRows; i++)
    {
        for(int j = 0; j < l->numCols; j++)
        {
            if(l->ladder[i][j] == n)
            {
                col = j;
            }
        }
    }
    return col;
}
int getUpperNeighbor(Ladder* l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);

    if(row <= 0)return -1;
    if(col < 0)return -1;


    for(int i = row-1; i >= 0; i--)
    {
        if(l->ladder[i][col] != 0)
        {
            return l->ladder[i][col];
        }
    }
    return -1;
}

int getRightNeighbor(Ladder* l, int n)
{
    int row = getRowIndex(l, n);
    int col = getColIndex(l, n);
    if(row <= 0)return -1;
    if(col < 0 || col >= l->numCols-1)return -1;

    row--;
    col++;
    for(int i = row; i >= 0; i--)
    {
        if(l->ladder[i][col] != 0)
        {
            return l->ladder[i][col];
        }
    }
    return -1;
}

void shiftLadderDown(Ladder *l, int dest, int source)
{
    forall(l->numCols)
    {
        l->ladder[dest][x] = l->ladder[source][x];
    }
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

    //printLadder(l);

    int un = getUpperNeighbor(l, 1);
    printf("%d\n", un);
    printf("right neigbor %d\n", getRightNeighbor(l, 1));
    
    //find the turn bar

    //call find all children with clean level = 1
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
    forall(l->numRows)
    {
        if (emptyRow(l, x))
            return x;
    }
    return MAXROWS;
}

bool canBeAddedToRow(Ladder *l, int row, int col)
{
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
