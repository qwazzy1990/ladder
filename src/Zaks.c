#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ladder.h"
#include "Zaks.h"
#include "utilities.h"

void suffixSize(int n, int *suffixVector)
{
    if (n == 2)
    {
        suffixVector[0] = 2;
        return;
        //append 2 to suffixVector
    }
    else
    {
        suffixSize(n - 1, suffixVector);
        int len = getLen(suffixVector);
        int *temp = calloc(len, sizeof(int));
        forall(len)
        {
            temp[x] = suffixVector[x];
        }
        int startIndex = len;
        forall(n - 1)
        {
            suffixVector[startIndex] = n;
            startIndex++;
            for (int i = 0; i < len; i++)
            {
                suffixVector[startIndex] = temp[i];
                startIndex++;
            }
            //append temp to suffixVector
        }
        free(temp);
    }
}

int getLen(int *a)
{
    int i = 0;
    for (i = 0; a[i] != -1; i++)
    {
    }

    return i;
}

//Zaks algorithm for generating permutations
void zaks(int** perms, int *perm, int n, int *suffixSizes, int idx)
{
    //if done then return
    if (suffixSizes[idx] == -1)
    {
       appendPerm(perms, perm, n);

        return;
    }

    //print the current perm
    appendPerm(perms, perm, n);

    //get the end index of the suffix
    int endIdx = n - 1;

    //get the start index of the suffix
    int startIdx = n - suffixSizes[idx];

    //reverse the suffix in the permutation
    forall((suffixSizes[idx]) / 2)
    {
        int temp = perm[endIdx];
        int temp2 = perm[startIdx];
        perm[endIdx] = temp2;
        perm[startIdx] = temp;
        endIdx--;
        startIdx++;
    }

    //recursive call
    idx++;
    zaks(perms, perm, n, suffixSizes, idx);
}

void appendPerm_real(int*** perms, int* perm, int n)
{
    int i = 0; 
    for(i = 0; (*perms)[i] != NULL; i++){

    }

    (*perms)[i] = calloc(n, sizeof(int));
    
    forall(n)
    {
        
        ((*perms)[i])[x] = perm[x];
    }
}


void genLaddersFromPerms(int** perms, int n)
{
    for(int i = 0; perms[i] != NULL; i++)
    {
        Ladder* l = newLadder(n);
        initLadder(l);
        printPerm(perms[i], n);
        l->ladderNumber = i+1;
        createRoot(l, perms[i], n, 0);
        printLadder(l);
        destroyLadder(l);
    }
}