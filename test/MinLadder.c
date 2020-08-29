#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"
#include "utilities.h"
#include "LinkedListAPI.h"

List *uninvert(int *perm, int n);
int getCount(int *perm, int start, int n);
void swapEven(int *perm, int start, int end, int n);
void getInput(int **perm, char *s);
void swapOdd(int *perm, List *perms, int start, int end, int n);

int getNumRows(int *perm, int n);
int numDig = 0;

static int *copyPerm(int *og, int n)
{
    new_object(int *, cpy, n);
    forall(n) cpy[x] = og[x];
    return cpy;
}

List *uninvert(int *perm, int n)
{
    new_object(int *, starts, n);
    new_object(int *, ends, n);

    forall(n) starts[x] = ends[x] = 0;

    int c = 0;
    for (int i = 0; i < n; i++)
    {
        //if there is an inversion
        if (i == n - 1)
            break;
        if (perm[i] > perm[i + 1])
        {
            int count = getCount(perm, i, n);
            starts[c] = i;
            ends[c] = i + count;
            i += count;
            c++;
        }
    }
    c = 0;
    int oddCount = 0;
    while (ends[c] != 0)
    {
        if ((ends[c] - starts[c]) % 2 == 1)
        {
            swapEven(perm, starts[c], ends[c], n);
        }
        else
        {
            oddCount++;
        }
        c++;
    }
    c = 0;
    int y = 0;
    List *oddList = initializeList(dummy_print, dummy_delete, dummy_compare);
    if (oddCount > 0)
    {
        while (ends[c] != 0)
        {
            if ((ends[c] - starts[c]) % 2 == 0 && y == 0)
            {

                int *cpy = copyPerm(perm, n);
                List *l = initializeList(dummy_print, dummy_delete, dummy_compare);
                swapOdd(cpy, l, starts[c], ends[c], n);
                insertBack(oddList, l);
                y++;
            }
            else if ((ends[c] - starts[c]) % 2 == 0 && y > 0)
            {
                List *l = getFromBack(oddList);
                List *newL = initializeList(dummy_print, dummy_delete, dummy_compare);
                for (Node *nn = l->head; nn != NULL; nn = nn->next)
                {
                    int *cpy = copyPerm(nn->data, n);
                    swapOdd(cpy, newL, starts[c], ends[c], n);
                }
                insertBack(oddList, newL);
            }

            c++;
        }
    }

    List *finList = getFromBack(oddList);
    return finList;
}

int getNumRows(int *perm, int n)
{
    int count = 0;
    printf("Getting the number of rows\n");
    while (!(isSorted(perm, n)))
    {
        printPerm(perm, n);
        forall(n)
        {
            if (x == n - 1)
                break;
            if (perm[x] > perm[x + 1])
            {
                swapInts(&(perm[x]), &(perm[x+1]));
                x++;
            }
        }
        count++;
    }
    printf("Done getting number of rows\nCount is %d\n",count);
    return count;
}

void swapOdd(int *perm, List *perms, int start, int end, int n)
{
    if (end - start == 2)
    {
        int *c1 = copyPerm(perm, n);
        int *c2 = copyPerm(perm, n);
        swapInts(&(c1)[start], &(c1)[start + 1]);
        swapInts(&(c2)[start + 1], &(c2)[end]);
        printPerm(c1, n);
        printPerm(c2, n);
        insertBack(perms, c1);
        insertBack(perms, c2);
        return;
    }
    else if (end - start == 1)
    {
        int *c1 = copyPerm(perm, n);
        swapInts(&(c1)[start], &(c1)[start + 1]);
        printPerm(c1, n);
        insertBack(perms, c1);
        return;
    }
    else if (start >= end)
        return;
    else
    {
        //swap first two

        int *c1 = copyPerm(perm, n);
        int *c2 = copyPerm(perm, n);
        swapInts(&(c1)[start], &(c1)[start + 1]);
        swapInts(&(c2)[start + 1], &(c2)[start + 2]);
        swapOdd(c1, perms, start + 2, end, n);

        swapOdd(c2, perms, start + 3, end, n);
    }
}

void swapEven(int *perm, int start, int end, int n)
{
    for (int i = start; i <= end; i += 2)
    {
        swapInts(&(perm[i]), &(perm[i + 1]));
    }
}
int getCount(int *perm, int start, int n)
{
    int count = 0;
    while (perm[start] > perm[start + 1])
    {
        count++;
        start++;
        if (start == n - 1)
            break;
    }
    return count;
}

void getInput(int **perm, char *s)
{
    *perm = calloc(1, sizeof(int));
    char delim[5];
    strcpy(delim, " \n");
    char *token = NULL;
    token = strtok(s, delim);
    int muls = 1;
    int memSize = 1;

    while (token != NULL)
    {

        int currNum = 0;
        for (int i = strlen(token) - 1; i >= 0; i--)
        {
            currNum += muls * (token[i] - 0x30);
            muls *= 10;
        }

        (*perm)[numDig] = currNum;

        numDig++;
        memSize++;
        muls = 1;

        token = strtok(NULL, delim);
        *perm = (int *)realloc(*perm, memSize * sizeof(int));
    }
}

int main()
{
    char *s = calloc(1000, sizeof(char));
    int *perm = NULL;

    printf("Enter a permutation\n");
    fgets(s, 1000, stdin);
    getInput(&perm, s);
    clear(s);

    List *iv = uninvert(perm, numDig);
    for(Node* n = iv->head; n!= NULL; n = n->next)
    {
       
        int numRows = getNumRows(n->data, numDig);
    }
    printPerm(perm, numDig);

    return 0;
}
