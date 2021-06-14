
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
    //int*  perm[7] = {3,1,4,6,2,7,5};
    int* p= calloc(7, sizeof(int));
    p[0] = 3;
    p[1] = 1;
    p[2] = 4;
    p[3] = 6;
    p[4] = 2;
    p[5] = 7;
    p[6] = 5;
    int N = 7;
    int numRows = 2*(N-1)-1;
    int numCols = N-1;
    bool** matrix = newMatrix(numRows, numCols);
    int n = N;
    while(n > 1)
    {
        int x = n;
        int posX = getPos(x, p, n);
        for(int i = 0; i < n; i++)
        {
            if(p[i] == x)continue;

            int posY = getPos(p[i], p, n);
            int* coordinates = getCoordinatesTwo(N, x, posX, posY);
            printf("(%d %d),(%d %d)\n", x, p[i], coordinates[0], coordinates[1]);
        }
        p = cpyA(p, x, n);
        n--;

    }
    // for(int i = 0; i < N-1; i++)
    // {
    //     for(int j = i+1; j < N; j++)
    //     {
            
    //         int x = max(perm[i], perm[j]);
    //         int y = min(perm[i], perm[j]);
    //         int posX = getPos(x, perm, N);
    //         int posY = getPos(y, perm, N);
    //         int* coordinates = getCoordinatesTwo(N, x, posX, posY);
    //         printf("(%d %d),(%d %d)\n", x, y, coordinates[0], coordinates[1]);
    //         if(x == perm[i])setMatrix(matrix, coordinates[0], coordinates[1], true);
    //         else setMatrix(matrix, coordinates[0], coordinates[1], false);
            

    //     }
    // }

   //printMatrix(matrix, numRows, numCols);

}