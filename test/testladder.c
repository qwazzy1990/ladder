#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"

bool DEBUG1 = true;

void getInput(int **perm, char *s);
int memSize = 1;
int numDig = 0;

int main(int argc, char *argv[])
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