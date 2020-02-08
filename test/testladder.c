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

bool DEBUG1 = true;
bool DEBUG7 = false;
bool DEBUG8 = false;
bool DEBUG9 = false;
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

       
        driver(perm, numDig);


        free(perm);
    }
    if (DEBUG7)
    {
        int n = 0;
        scanf("%d", &n);
        int **perms = calloc(fact(n) * fact(n), sizeof(int));
        int * perm = calloc(n, sizeof(int));
        int* arr = calloc(n, sizeof(int));
        bool* direction = calloc(n, sizeof(int));
        forall(n)
        {
            perm[x] = x+1;
            arr[x] = 0;
            direction[x] = false;
        }


        forall(fact(n)*2)
            perms[x] = calloc(n, sizeof(int));
       
        genPermsSJT(perms, perm, n, arr, direction);
       
       
        for(int i = 0; i < fact(n); i++)
        {
            printf("    %d\n", i + 1);

            printPerm(perms[i], n);
            Ladder *l = newLadder(n);
            initLadder(l);
            createRoot(l, perms[i], n, 0);

            printLadder(l);
            destroyLadder(l);
        }
        for (int i = 0; i < fact(n)*2; i++)
            free(perms[i]);

        free(perms);
        free(perm);
        free(arr);
        free(direction);

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

   if(DEBUG9)
   {
       int* perm = calloc(4, sizeof(int));
       forall(4){
        perm[x] = 4-x;
       }

        int n = countDegenerativeSubsequences(perm, 4);
        new_object(DTSA*, dts, n);
        setDTSs(dts, perm, n, 4);
        
        freeDts(dts, n);
        free(perm);
     
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
