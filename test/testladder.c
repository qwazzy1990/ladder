#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"
#include "Sjt.h"
#include "svg.h"
#include "LinkedListAPI.h"

bool DEBUG1 = false;
bool DEBUG7 = true;
bool DEBUG8 = false;
int memSize = 1;
int numDig = 0;

void getInput(int **perm, char *s);

int main(int argc, char *argv[])
{
    if (DEBUG1)
    {
        char *s = calloc(1000, sizeof(char));
        int *perm = NULL;

        printf("Enter a permutation\n");
        fgets(s, 1000, stdin);
        getInput(&perm, s);
        clear(s);

        printf("Input Permutation:\n");
        forall(numDig)
        {
            printf(GREEN " %d " COLOR_RESET, perm[x]);
        }
        printf("\n");

        List *list = initializeList(ladderToString, destroyLadder, dummy_compare);

        driver(list, perm, numDig);

        freeList(list);

        free(perm);
    }
    if (DEBUG7)
    {
        int n = 0;
        printf("Enter a number between 1 and 9\n");
        scanf("%d", &n);
        new_object(int**, perms, fact(n)*2);
        bool* direction = calloc(n, sizeof(bool));
        int* arr = calloc(n, sizeof(arr));
        int* perm = calloc(n, sizeof(int));
        forall(n)
        {
            direction[x] = RIGHT;
            arr[x] = 0;
            perm[x] = x+1;
        }

       
        genPermsSJTReverse(perms, perm, n, arr, direction);
       
       
        for (int i = 0; i < fact(n); i++)
        {
            printf("    %d\n", i + 1);

            printPerm(perms[i], n);
            Ladder *l = newLadder(n);
            initLadder(l);
            printf("rows %d, cols %d, height %d\n", l->numRows, l->numCols, l->depth);
            createRoot(l, perms[i], n, 0);
            printf("rows %d, cols %d, height %d\n", l->numRows, l->numCols, l->depth);

            printLadder(l);
            destroyLadder(l);
        }
        for (int i = 0; i < fact(n); i++)
            free(perms[i]);

        free(perms);

    }
    if(DEBUG8)
    {
        int n = 5;
        bool* direction = calloc(n, sizeof(bool));
        int* arr = calloc(n, sizeof(arr));
        forall(n)
        {
            direction[x] = true;
            arr[x] = 0;
        }
        Ladder* l = newLadder(n);
        initLadder(l);
        sjtLadder(l, n, arr, direction);
        destroyLadder(l);
        free(arr);
        free(direction);
    }

   
}

void getInput(int **perm, char *s)
{
    *perm = calloc(memSize, sizeof(int));
    forall(strlen(s))
    {
        if (isdigit(s[x]))
        {
            (*perm)[numDig] = s[x] - 0x30;
            numDig++;
            memSize++;
            *perm = (int *)realloc(*perm, memSize * sizeof(int));
        }
    }
}