#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ladder.h"
#include "utilities.h"

Ladder *newLadder(int *perm, int size)
{
    new_object(Ladder *, l, 1);

    l->ladder = NULL;
    l->numRows = MAXROWS;
    l->numCols = size - 1;
    return l;
}

void initLadder(Ladder *l)
{
    l->ladder = calloc(MAXROWS, sizeof(Bar *));
    forall(l->numRows)
    {
        l->ladder[x] = calloc(l->numRows, sizeof(Bar));
    }

    for (int i = 0; i < l->numRows; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            setDummy(l, i, j);
        }
    }
}

void setDummy(Ladder *l, int rowNum, int colNum)
{
    l->ladder[rowNum][colNum].set = false;
    l->ladder[rowNum][colNum].vals[0] = 0;
    l->ladder[rowNum][colNum].vals[1] = 0;
    l->ladder[rowNum][colNum].routeNum = 0;
    l->ladder[rowNum][colNum].currRow = rowNum;
    l->ladder[rowNum][colNum].prevRow = rowNum;

    l->ladder[rowNum][colNum].currCol = colNum;
    l->ladder[rowNum][colNum].prevCol = colNum;
}