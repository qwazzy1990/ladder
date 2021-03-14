
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


int getPos( int X, int* perm, int N);
int getPos( int X, int* perm, int N)
{
    forall(N)
    {
        if(perm[x] == X)return x;
    }
    return -1;
}

int max(int x, int y);
int max(int x, int y)
{
    return x > y ? x:y;
}

int min(int x, int y);
int min(int x, int y)
{
    return x < y ? x:y;
}

//get the number of elements in pi less than x and to the right of x 
int RX(int x, int* perm, int N);
int RX(int x, int* perm, int N)
{
    int idx = getPos(x, perm, N);
    int rx = 0;
    for(int i = idx+1; i < N; i++)
    {
        if(x > perm[i])rx++;
    }
    return rx;

}

int main()
{
    int perm[7] = {3,1,4,6,2,7,5};
    int N = 7;

    int x = max(perm[3], perm[4]);

    int y = min(perm[3], perm[4]);
    printf("x is %d y is %d\n", x, y);
    int posX = getPos(x, perm, N);
    int posY = getPos(y, perm, N);
    int rX = RX(x, perm, N);
    printf("%d\n", rX);


    int* coordinates = getBarCoordinates(N, x, y, posX, posY, &rX);
    printf("(%d,%d)\n", coordinates[0], coordinates[1]);

}