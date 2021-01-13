

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
bool _DEBUG2 = false;
bool _DEBUG3 = false;
bool DEBUG4 = true;

float xAxis = 0.0;
float yAxis = 6.0;

int rowCounter = 0;


void sjtMod(Ladder *l, int count, int n, bool *dir);
void printLadderTikz(Ladder *l);
int calcDepth(Ladder* l);
int calcDepth(Ladder* l)
{
    int i = 0;
    int j = 0;
    bool flag = true;
    while(1)
    {
        for(int i = 0; i < l->numCols; i++)
        {
            if(l->ladder[j][i] == 1)
            {
                flag = false;
                break;
            }
        }
        if(flag == true)
            return j+1;
        else{
            flag = true;
            j++;
        }

    }
}



void printLadderTikz(Ladder *l)
{
    //    printf("\\begin{figure}[!htp]");
    //   printf("\\begin{tikzpicture}");

    float c1 = xAxis;
    float c2 = yAxis;
    for (int i = 0; i <= l->numCols; i++)
    {
        printf("\\draw(%.2f,%.2f) to (%.2f,%.2f);", c1, c2 -.1, c1, c2 + 1.1);
        printf("\n");
        c1 += 0.2;
    }
    c1 = xAxis;
    c2 = c2 + 1.0;
    for (int i = 0; i <= 2*(l->numCols)-1; i++)
    {
         for (int j = 0; j < l->numCols; j++)
         {
             if (l->ladder[i][j] > 0)
             {
                 printf("\\draw(%.2f, %.2f) to (%.2f, %.2f);", c1, c2, c1 + .2, c2);
                 printf("\n");
             }
             c1 += .2;
         }
         c2 -= .2;
         c1 = xAxis;
    }
    // c1 = *cA;
    // forall(numDig)
    // {
    //     printf("\\node at(%.2f,%.2f){\\tiny{%d}};", c1, (*cB) + .4, perm[x]);
    //     c1 += .2;
    // }

    //printf("\\end{tikzpicture}");
    //printf("\\end{figure}");
    xAxis += 1.0;
    //*cB += -1.5;
}


void sjtMod(Ladder *l, int count, int n, bool *dir)
{
    //if count > than max value=n
    if (count > n)
    {
        if(rowCounter % 6 == 0 && rowCounter > 0)
        {
            xAxis = 0.0;
            yAxis -= 1.5;
        }
        printLadderTikz(l);
        printf("\n\n");
        rowCounter++;
        return;
    }

    for(int i = -1; i < count-1; i++)
    {
        //don't add or remove a bar on first iteration of the loop, just do recursive call
        if (i == -1)
            sjtMod(l, count + 1, n, dir);
        //if second or greater iteration then add or remove a bar
        else
        {
            //if adding a bar then go right to left
            if (dir[count - 1] == true)
            {
                int row = (n - 2) + (n - count) - (i);
                int col = (count - 2) - (i);
                l->ladder[row][col] = 1;
            }
            //if removing a bar then go left to right
            else
            {
                int col = (i);
                int row = 2 * (n - count) + (i);
                l->ladder[row][col] = 0;
            }
            //recursive call
            sjtMod(l, count+1, n, dir);
        }
    }
    //switch the direction from right to left or left to right
    dir[count - 1] = !(dir[count - 1]);
}

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
            //insertAt(l, n - 1, reservedRows, i);
        }

        for (int i = n - 2; i >= 0; i--)
        {
            // insertAt(l, n, reservedRows, i);
        }
        printf("%d %d %d %d\n", l->ladder[0][0], l->ladder[1][1], l->ladder[2][2], l->ladder[3][3]);

        destroyLadder(l);
        free(reservedRows);
    }

    //gray code for generating permutations based on sjt. Permutations used to generate ladders
    if (_DEBUG2)
    {
        int n = 0;
        scanf("%d", &n);
        new_object(int **, perms, fact(n) * 2);
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        int *perm = calloc(n, sizeof(int));
        forall(n)
        {
            direction[x] = RIGHT;
            arr[x] = 0;
            perm[x] = x + 1;
        }

        forall(fact(n) * 2)
            perms[x] = calloc(n, sizeof(int));

        //genPermsSJTReverse(perms, perm, n, arr, direction);

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
        for (int i = 0; i < fact(n) * 2; i++)
            free(perms[i]);

        free(perms);
        free(perm);
        free(arr);
        free(direction);
    }

    if (_DEBUG3)
    {
        int n = 0;
        scanf("%d", &n);
        new_object(int **, perms, fact(n) * 2);
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        int *perm = calloc(n, sizeof(int));
        forall(n)
        {
            direction[x] = LEFT;
            arr[x] = 0;
            perm[x] = x + 1;
        }
        forall(fact(n) * 2)
            perms[x] = calloc(n, sizeof(int));

        //genPermsSJT(perms, perm, n, arr, direction);
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
        for (int i = 0; i < fact(n) * 2; i++)
            free(perms[i]);

        free(perms);
        free(perm);
        free(arr);
        free(direction);
    }
    if (DEBUG4)
    {
        int n = 4;
        bool *dir = calloc(n, sizeof(bool));
        for (int i = 2; i <= n; i++)
        {
            dir[i - 1] = true;
        }
        Ladder *l = newLadder(n);
        initLadder(l);
        sjtMod(l, 2, n, dir);
    }

    return 0;
}