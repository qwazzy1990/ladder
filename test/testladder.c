#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

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
#include "HashMap.h"
//for the mid ladder problem
bool DEBUG0 = true;
bool DEBUG1 = false;
//for the enumeration problem
bool DEBUG7 = false;
//for the sjt problem
bool DEBUG8 = false;
//for counting degenerative triadic subsequences
bool DEBUG9 = false;
//for heaps ladders
bool DEBUG10 = false;
//for the gray code problem using all 4 algorithms
bool DEBUG11 = false;
int memSize = 1;
int numDig = 0;

void getInput(int **perm, char *s);

void testDriverSJT(int n);

void runMinLadderExperiment(int start, int end, int numRuns, int mapSize);

int *generateRandomPerm(int n, HashMap *h);

void runMinLadderExperiment(int start, int end, int numRuns, int mapSize)
{
    HashMap *hMap = newMap(1000, dummy_print, &free, &strcmp);

    for (int i = start; i <= end; i++)
    {
        for (int j = 0; j < numRuns; j++)
        {
            
        
                int *perm = generateRandomPerm(i, hMap);

                printf("Input Permutation:\n");
                forall(i)
                {
                    printf(GREEN " %d " COLOR_RESET, perm[x]);
                }
                printf("\n");

                genMinLadders(perm, i);
                minLadderDriver(perm, i);
                free(perm);
            
        }
    }
    deleteHashMap(hMap);
}

int *generateRandomPerm(int n, HashMap *h)
{
    new_object(int *, perm, n);
    forall(n) perm[x] = 0;
    int numDig = 0;
    time_t t;
    bool repeat = false;

    /* Intializes random number generator */
    srand((unsigned)time(&t));

    while (1)
    {
        int randDig = (rand() % n) + 1;
        for (int i = 0; i < numDig; i++)
        {
            if (perm[i] == randDig)
            {
                repeat = true;
                break;
            }
        }
        if (!repeat)
        {
            perm[numDig] = randDig;
            numDig++;
        }
        else
        {
            repeat = !repeat;
        }
        if (numDig == n)
        {
            char *s = calloc(n + 2, sizeof(char));
            forall(n)
            {
                s[x] = perm[x] + '0';
            }
            if (containsData(h, s) == false)
            {
                insertToMap(h, s);
                return perm;
            }
            else
            {
                free(perm);
                free(s);
                perm = calloc(n, sizeof(int));
                numDig = 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    PRINT = false;
    if (DEBUG0)
    {
        runMinLadderExperiment(4, 7, 5, 1000);
    }
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
        minLadderDriver(perm, numDig);
        free(perm);
    }
    if (DEBUG7)
    {
        char *s = calloc(1000, sizeof(char));
        int *perm = NULL;

        printf("Enter a permutation\n");
        fgets(s, 1000, stdin);
        getInput(&perm, s);
        printf("Number of digits is %d\n", numDig);
        clear(s);

        printf("Input Permutation:\n");
        forall(numDig)
        {
            printf(GREEN " %d " COLOR_RESET, perm[x]);
        }
        printf("\n");
        saveAllLadders(perm, numDig);
        free(perm);
    }
    if (DEBUG8)
    {
        printf("Enter an integer greater than or equal to 1\n");
        int n;
        scanf("%d", &n);
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        new_object(int *, perm, n);
        forall(n)
        {
            direction[x] = false;
            arr[x] = 0;
            perm[x] = x + 1;
        }
        List *perms = initializeList(dummy_print, free, dummy_compare);
        genPermsSJT(perms, perm, n, arr, direction);
        //printAllPerms(perms, n);
        Node *node = perms->head;
        //printPerm(perms->head->data, n);
        Node *tempHead = perms->head;
        Ladder *l = newLadder(n);
        forall(n)
        {
            direction[x] = false;
            arr[x] = 0;
        }
        initLadder(l);
        sjtLadder(l, &node, n, arr, direction);
        perms->head = tempHead;
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
    char delim[5];
    strcpy(delim, " \n");
    char *token = NULL;
    token = strtok(s, delim);
    int muls = 1;

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
    // forall(strlen(s))
    // {
    //     if (isdigit(s[x]))
    //     {
    //         (*perm)[numDig] = s[x] - 0x30;
    //         numDig++;
    //         memSize++;
    //         *perm = (int *)realloc(*perm, memSize * sizeof(int));
    //     }
    // }
}
