

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
bool DEBUG2 = true;
int main()
{

    if (DEBUG1)
    {
        int n = 4;
        Ladder *l = newLadder(n);
        initLadder(l);

        int *reservedRows = calloc(n, sizeof(int));
        reserveRows(reservedRows, n);
        printf("%d\n", reservedRows[2]);

        for (int i = n - 3; i >= 0; i--)
        {
            insertAt(l, n - 1, reservedRows, i);
        }

        for (int i = n - 2; i >= 0; i--)
        {
            insertAt(l, n, reservedRows, i);
        }
        printf("%d %d %d %d\n", l->ladder[0][0], l->ladder[1][1], l->ladder[2][2], l->ladder[3][3]);

        destroyLadder(l);
        free(reservedRows);
    }
    if(DEBUG2)
    {
        int n = 4;
        bool* direction = calloc(n, sizeof(bool));
        int* arr = calloc(n, sizeof(arr));
        int* perm = calloc(n, sizeof(int));
        new_object(int**, perms, fact(n)*2);

        forall(n)
        {
            direction[x] = RIGHT;
            arr[x] = 0;
            perm[x] = x+1;
        }
      
        genPermsSJTReverse(perms, perm, n, arr, direction);
        free(arr);
        free(direction);
        free(perm);

    }

    return 0;
}