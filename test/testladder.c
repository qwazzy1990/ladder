#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"
#include "Sjt.h"

bool DEBUG1 = false;
bool DEBUG2 = false;
bool DEBUG7 = true;

void getInput(int **perm, char *s);
int memSize = 1;
int numDig = 0;

void sjt(int *perm, int count, int max, int size, bool flag);
void printPerm(int *perm, int size);
int factorial(int n);
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

void printPerm(int *perm, int size)
{
    forall(size)
    {
        printf("%d     ", perm[x]);
    }
    printf("\n");
}

void sjt(int *perm, int count, int max, int size, bool flag)
{
    printf("max is %d size is %d\n", max, size);
    printPerm(perm, size);
    Ladder *ll = newLadder(size);
    printf("Ladder Number = %d\n", count + 1);

    initLadder(ll);
    createRoot(ll, perm, size, 0);
    printLadder(ll);
    destroyLadder(ll);

    if (count >= max)
        return;
    if (flag == true)
    {
        forall(size - 1)
        {
            int temp = perm[x];
            perm[x] = perm[x + 1];
            perm[x + 1] = temp;
            printPerm(perm, size);
            count++;
            printf("Ladder number = %d\n", count + 1);
            Ladder *ll = newLadder(size);
            initLadder(ll);
            createRoot(ll, perm, size, 0);
            printLadder(ll);
            destroyLadder(ll);

            if (count == max)
                return;
        }
        int temp = perm[0];
        perm[0] = perm[1];
        perm[1] = temp;
        count++;
        flag = false;
        sjt(perm, count, max, size, flag);
        return;
    }
    if (flag == false)
    {
        for (int i = size - 1; i > 0; i--)
        {
            int temp = perm[i];
            perm[i] = perm[i - 1];
            perm[i - 1] = temp;
            printPerm(perm, size);
            count++;

            printf("Ladder number = %d\n", count + 1);

            Ladder *ll = newLadder(size);
            initLadder(ll);
            createRoot(ll, perm, size, 0);
            printLadder(ll);
            destroyLadder(ll);

            if (count == max)
                return;
        }
        int temp = perm[size - 1];
        perm[size - 1] = perm[size - 2];
        perm[size - 2] = temp;
        count++;
        // if (count >= max)
        //     return;
        flag = true;
        sjt(perm, count, max, size, flag);
        return;
    }
}
