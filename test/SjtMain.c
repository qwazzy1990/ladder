

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

bool _DEBUG1 = false;
bool _DEBUG2 = true;
bool _DEBUG3 = false;

int main()
{

    //gray code for generating ladders based on sjt
    if (_DEBUG1)
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

    //gray code for generating permutations based on sjt. Permutations used to generate ladders
    if(_DEBUG2)
    {
        int n = 0;
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

        forall(fact(n)*2)
            perms[x] = calloc(n, sizeof(int));
       
        genPermsSJTReverse(perms, perm, n, arr, direction);
       
       
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

    if(_DEBUG3)
    {
        int n = 0;
        scanf("%d", &n);
        new_object(int**, perms, fact(n)*2);
        bool* direction = calloc(n, sizeof(bool));
        int* arr = calloc(n, sizeof(arr));
        int* perm = calloc(n, sizeof(int));
        forall(n)
        {
            direction[x] = LEFT;
            arr[x] = 0;
            perm[x] = x+1;
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

    return 0;
}