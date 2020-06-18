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
#include "Zaks.h"
#include "Heaps.h"
#include "LinkedListAPI.h"
#include "ehr.h"
#include "lexicographic.h"
#include "GrayCode.h"

bool DEBUG1 = true;
bool DEBUG7 = false;
bool DEBUG8 = false;
bool DEBUG9 = false;
bool DEBUG10 = false;
bool DEBUG11 = false;
int memSize = 1;
int numDig = 0;

void getInput(int **perm, char *s);

void testDriverSJT(int n);

static bool allUnique(List *l, int nn)
{
    int count = 0;
    int cc = 0;
    for (Node *h = l->head; h != NULL; h = h->next)
    {
        for (Node *n = h->next; n != NULL; n = n->next)
        {
            //if(n == NULL)return true;
            int *p1 = h->data;
            int *p2 = n->data;
            forall(nn)
            {
                if (p1[x] == p2[x])
                {
                    count++;
                }
            }
            if(count == nn)
            {
                printf("perms at %d %d\n", cc, cc+1);
                printf("%p %p\n", (void*)h, (void*)n);
                printPerm(p1, nn);
                printPerm(p2, nn);
                return false;
            }
            count = 0;
           
        }
        cc++;
        
    }
    return true;
}

int main(int argc, char *argv[])
{
    PRINT = false;
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

        genMinLadders(perm, numDig);
        //saveAllLadders(perm, numDig);
        //encodingDriver(perm, numDig, 1);
        PRINT = true;
       // genMinLadders(perm, numDig);
       //driver(perm, numDig);
        genMinLadders(perm, numDig);
        PRINT = false;

        free(perm);
        
    }
    if (DEBUG7)
    {
    }
    if (DEBUG8)
    {
        int n = 5;
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        new_object(int*, perm, n);
        forall(n)
        {
            direction[x] = false;
            arr[x] = 0;
            perm[x] = x+1;
        }
        List* perms = initializeList(dummy_print, free, dummy_compare);
        genPermsSJT(perms, perm, n, arr, direction);
        printAllPerms(perms, n);
        Ladder *l = newLadder(n);
        forall(n)
        {
            direction[x] = false;
            arr[x] = 0;
        }
        initLadder(l);
        sjtLadder(l, n, arr, direction);
        destroyLadder(l);
        free(arr);
        free(direction);
        free(perm);
        freeList(perms);
    }

    if (DEBUG9)
    {
        int *perm = calloc(4, sizeof(int));
        forall(4)
        {
            perm[x] = 4 - x;
        }

        int n = countDegenerativeSubsequences(perm, 4);
        new_object(DTSA *, dts, n);
        setDTSs(dts, perm, n, 4);

        freeDts(dts, n);
        free(perm);
    }

    if (DEBUG10)
    {
        int n = 4;
        List *l = initializeList(dummy_print, free, dummy_compare);
        new_object(int *, perm, 4);
        forall(n) perm[x] = x + 1;

        heaps(l, perm, n, n - 1);
        printAllPerms(l, n);
        freeList(l);
        free(perm);
    }
    if (DEBUG11)
    {
        int n = 7;

        grayCodeDriver(n);
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
