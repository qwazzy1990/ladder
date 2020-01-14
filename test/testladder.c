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
bool DEBUG2 = false;
bool DEBUG7 = false;
bool DEBUG8 = true;
int memSize = 1;
int numDig = 0;

int factorial(int n);
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
    if (DEBUG2)
    {
        printf("Enter a number between 1 and 9\n");
        int n = -1;
        scanf("%d", &n);
        int *perm = calloc(100, sizeof(int));
        forall(n)
        {
            perm[x] = x + 1;
            printf("%d ", perm[x]);
        }
        printf("\n");

        sjt(perm, 0, factorial(n), n, true);
        free(perm);
    }
    if (DEBUG7)
    {
        int n = 0;
        printf("Enter a number between 1 and 9\n");
        scanf("%d", &n);
        int **perms = calloc(fact(n) * fact(n), sizeof(int));
        printPerms(perms, n);
        for (int i = 0; i < fact(n); i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", perms[i][j]);
            }
            printf("\n");
        }

        for (int i = 0; i < fact(n); i++)
        {
            printf("    %d\n", i + 1);

            printPerm(perms[i], n);
            Ladder *l = newLadder(n);
            initLadder(l);
            createRoot(l, perms[i], n, 0);
            printLadder(l);
            destroyLadder(l);
        }
        for (int i = 0; i < fact(n); i++)
            free(perms[i]);

        free(perms);

        printf("<line x1=\"20\" y1=\"0\" x2=\"20\" y2=\"200\" style=\"stroke:rgb(255,0,0);stroke-width:2\"/><line x1=\"20\" y1=\"20\" x2=\"60\" y2=\"20\" style=\"stroke:rgb(255,0,0);stroke-width:2\"/>");
    }

    if (DEBUG8)
    {
        char *s = calloc(1000, sizeof(char));
        int *perm = NULL;
        fgets(s, 1000, stdin);
        getInput(&perm, s);
        clear(s);
        List *list = initializeList(ladderToString, destroyLadder, dummy_compare);

        driver(list, perm, numDig);

        s = htmlBody(list);
        printf("%s\n", s);
        freeList(list);

        free(perm);
    }
}

int factorial(int n)
{
    int result = 1;
    for (int i = n; i >= 1; i--)
    {
        result = result * i;
    }
    printf("Result is %d\n", result);
    return result;
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
