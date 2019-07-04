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
    return l;
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
    l->bars = calloc(100, sizeof(Bar));
}

void createRoot(Ladder *l, int *perm, int size, int currRow)
{
    if (size == 1)
        return;

    int largestIndex = getLargestIndex(perm, size);

    foreach (largestIndex, size)
    {
        //if an inversion is found
        if (perm[largestIndex] > perm[x])
        {
            int col = x - 1;
            int row = getFirstAvailableRow(l, currRow, col);
            currRow = row;
            setBar(&(l->bars[l->numBars]), l->numBars + 1, perm[largestIndex], perm[x]);
            l->numBars++;
            l->ladder[row][col] = l->numBars;

            //col index is 0 or x-1
            //
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
    foreach(currRow, l->numRows)
    {
        if (canBeAddedToRow(l, x, col))
        {
            return x;
        }
    }
    return -1;
}

/*Printers */
char *printBar(Bar b)
{
    char *s = calloc(1000, sizeof(char));
    char temp[100];
    sprintf(temp, "%d", b.barNum);
    strcat(s, "Bar Number: ");
    strcat(s, temp);
    sprintf(temp, "%d", b.vals[0]);
    strcat(s, "Values:");
    strcat(s, temp);
    strcat(s, " ");
    sprintf(temp, "%d", b.vals[1]);
    strcat(s, temp);
    strcat(s, "\n\n");
    return s;
}

void printLadder(Ladder* l)
{
    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if(val == 0)
                printf("%d ", val);
            else
            {
                printf(GREEN "%d " COLOR_RESET, val);
            }
            
        }
        printf("\n");
    }

    forall(l->numBars)
    {
        char* s = printBar(l->bars[x]);
        printf("%s", s);
        free(s);
    }
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

void driver(int *perm, int size)
{
    //create a ladder
    Ladder *l = newLadder(size);

    //initialize it
    initLadder(l);

    createRoot(l, perm, size, 0);

    printLadder(l);

    //find the turn bar

    //call find all children with clean level = 1
}

bool emptyCell(Ladder *l, int row, int col)
{
    if (l->ladder[row][col] == 0)
        return true;
    return false;
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
