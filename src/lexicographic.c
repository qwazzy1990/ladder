#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "ladder.h"
#include "lexicographic.h"

#include "LinkedListAPI.h"

void orderedPerms(List *perms, int *perm, int nFactorial, int count, int size)
{

    insertBack(perms, copyIntArr(perm, size));

    if (count == nFactorial)
    {
        return;
    }

    for (int i = size - 1; i > 0; i--)
    {
        if (perm[i - 1] < perm[i])
        {
            int index = getSwapIndex(perm, i - 1, size);
            swap(&(perm[i - 1]), &(perm[index]), swapInts);
            sortRightPart(perm, i, size);
            orderedPerms(perms, perm, nFactorial, count + 1, size);
        }
    }
}

int getSwapIndex(int *perm, int currIndex, int size)
{
    unsigned long smallestGreater = 100000000000;
    int swapIndex = -1;

    foreach (currIndex, size)
    {
        if (perm[x] > perm[currIndex])
        {
            if (perm[x] < smallestGreater)
            {
                smallestGreater = perm[x];
                swapIndex = x;
            }
        }
    }
    return swapIndex;
}

void sortRightPart(int *perm, int start, int end)
{

    for (int i = start; i < end; i++)
        for (int j = i + 1; j < end; j++)
            if (perm[i] > perm[j])
            {
                swap(&(perm[i]), &(perm[j]), swapInts);
            }
}