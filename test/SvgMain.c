

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

int memSize = 1;
int numDig = 0;
void getInput(int **perm, char *s);

int main()
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