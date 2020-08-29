//this algorithm generates all permutations of size N by creating cycles of permutations
//with 0...k inversions where K = N(N-1)/2. The maximal number of inversions for a permutation of size N.
//author: Patrick Di Salvo
//Date: July 10th 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "ladder.h"
#include "Sjt.h"

//for printing
bool Print = false;
int NFact = 0;
int GlobCount = 0;

struct perm
{
    int *perm;
    int numInversions;
};
typedef struct perm Perm;

int globCount = 1;
bool _DIRECTION_ = true;
//creates a permutation with k inversions, the number of elements, E, in the new permutation is
//equal to the smallest E that satisfies the equation E(E-1)/2 >= k .
Perm *newPerm(int k);

void funcDriver(Ladder *l, int maxLimit);
//The ladder, the current limit indicating the desired current number of bars to put in the ladder,
//maxLimit = (n*(n-1))/2 = the maximum number of inversions allowed in a ladder with N columns.
//a ladder with maxLimit inversions and n columns is unique..
//current column is the current column to add to
//current row is the current row to add to
//the array holds the number of times to loop for each column in the ladder
void func(Ladder *l, int currLimit, int maxLimit, int currCol, int currRow, int *arr);

void funcTwo(Ladder *l, int currLimit, int maxLimit, int k, int n);
void funcDriverTwo(Ladder *l, int n);

void removeAllBars(Ladder *l);
void __addBar(Ladder *l, int row, int col);
void removeSomeBars(Ladder *l, int currRow, int currCol);
void printLadderDisalvo(Ladder *l);
int calcOffset(Ladder *l);
void HelperSJT(Ladder *l, int n, int *arr, bool *dir);
void SJTLadder(Ladder *l, int n, int *arr, bool *dir);
void SJTDriver(Ladder *l, int n);

int calcOffset(Ladder *l)
{
    int offset = l->numCols;
    offset = offset*2;
    return offset+1;

}

//prints the ladder
void printLadderDisalvo(Ladder *l)
{
    if (Print == false)
        return;
    int offset = calcOffset(l);
    for (int i = 0; i < offset; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            printf("|");
            if (l->ladder[i][j] == 1)
                printf("-----");
            else
                printf("     ");
        }
        printf("|");
        printf("\n");
    }
}
//add a bar at a certain row col
void __addBar(Ladder *l, int row, int col)
{
    l->ladder[row][col] = 1;
    l->numBars++;
}

//removes all the bars when you increment the current limit
void removeAllBars(Ladder *l)
{
    for (int i = 0; i < l->numCols + calcOffset(l) + 1; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            if (l->ladder[i][j] == 1)
                l->ladder[i][j] = 0;
        }
    }
    l->numBars = 0;
}

//removes some of the bars from the ladder belonging to a specific diagnonal
void removeSomeBars(Ladder *l, int currRow, int currCol)
{
    if (currRow < 0 || currCol < 0)
        return;
    if (l->ladder[currRow][currCol] == 0)
        return;

    l->ladder[currRow][currCol] = 0;
    l->numBars--;
    if (_DIRECTION_)
        removeSomeBars(l, currRow - 1, currCol - 1);
    if (!_DIRECTION_)
        removeSomeBars(l, currRow + 1, currCol + 1);
}

void funcTwo(Ladder *l, int currLimit, int maxLimit, int k, int n)
{
    //if you have a ladder with the correct number of
    //bars then print it and return
    if (l->numBars == currLimit)
    {
        if (Print)
        {
            printf("Ladder Number: %d\n", globCount);
            printLadderDisalvo(l);
            printf("\n");
        }
        //removeAllBars(l);
        globCount++;
        return;
    }

    //if the current limit is at the max limit then return. This is the last ladder
    //in the set.
    if (currLimit > maxLimit)
        return;

    if (k == n)
    {
        int row = k - 1;
        int col = row - 1;
        while (l->numBars < currLimit && col >= 0)
        {
            __addBar(l, row, col);
            row--;
            col--;
        }
        //remove the bars just added and return
        if (l->numBars == currLimit)
        {
            if (Print)
            {
                printf("Ladder Number: %d\n", globCount);
                printLadderDisalvo(l);
                printf("\n");
            }
            globCount++;
        }
        row = k - 1;
        col = row - 1;
        removeSomeBars(l, row, col);

        return;
    }
    else
    {
        //if you are not at the last column then
        int count = 0;
        int row = 0;
        int col = 0;
        //go to the array at the current column
        for (int i = 0; i < k; i++)
        {
           
            if (i == 0)
            {
                funcTwo(l, currLimit, maxLimit, k + 1, n);
                //continue;
            }
            if(l->numBars == currLimit)break;

            else
            {

                int row = (n - 1) + (n - k) - count;
                int col = (k)-2 - count;
                __addBar(l, row, col);
                count++;
                funcTwo(l, currLimit, maxLimit, k + 1, n);
            }
        }
        //once you have added all the bars for the current column, from bot left to top right, remove them all
        row = (n - 1) + (n - k);
        col = (k)-2;
        removeSomeBars(l, row, col);
    }
}

void funcDriverTwo(Ladder *l, int n)
{

    int maxLimit = (n * (n - 1)) / 2;
    clock_t begin = clock();

    for (int i = 0; i <= maxLimit; i++)
    {
        funcTwo(l, i, maxLimit, 2, n);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("--TIME SPENT: %f---\n", time_spent);
}

void SJTDriver(Ladder *l, int n)
{
    bool *dir = calloc(n, sizeof(bool));
    int *arr = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
    {
        dir[i] = false;
        arr[i] = 0;
    }
    clock_t begin = clock();
    while (GlobCount < NFact)
    {
        SJTLadder(l, n, arr, dir);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("--TIME SPENT: %f---\n", time_spent);
    free(dir);
    free(arr);
}

void removeBar(Ladder *l, int row, int col)
{
    l->ladder[row][col] = 0;
}
void SJTLadder(Ladder *l, int n, int *arr, bool *dir)
{
    GlobCount++;
    if (Print)
    {
        printf("Ladder Number: %d\n", GlobCount);
        printLadderDisalvo(l);
        printf("\n");
    }
    for (int i = 0; i < n - 1; i++)
    {
        //adding a bar
        if (dir[n - 1] == false)
        {
            __addBar(l, (n - 1) - i, (n - 2) - i);
        }
        //removing a bar
        else
        {
            removeBar(l, i + 1, i);
        }
        GlobCount++;
        if (Print)
        {
            printf("Ladder Number: %d\n", GlobCount);
            printLadderDisalvo(l);
            printf("\n");
        }
    }

   
    dir[n - 1] = !(dir[n - 1]);
    HelperSJT(l, n, arr, dir);
}

void HelperSJT(Ladder *l, int n, int *arr, bool *dir)
{
    for (int i = n - 1; i > 1; i--)
    {
        //if adding or removing from Nth elem
        if (arr[i - 1] < i - 1)
        {
            //if adding
            if (dir[i - 1] == false)
            {
                int row = (n - 1) + (n - i) - arr[i - 1];
                int col = (i - 1) - arr[i - 1] - 1;
                __addBar(l, row, col);
            }
            //if removing
            else
            {
                int row = (n - 1) + (n - i) + arr[i - 1] - (i - 2);
                int col = arr[i - 1];
                removeBar(l, row, col);
            }
            arr[i - 1]++;
            return;
        }
        else
        {
            arr[i - 1] = 0;
            dir[i - 1] = !(dir[i - 1]);
        }
    }
}
int main()
{
    int n;
    int p = 0;
    printf("Enter a number >= 3\n");
    scanf("%d", &n);
    printf("Enter 1 to turn printing on\nEnter 0 to turn printing off\n");
    scanf("%d", &p);
    (p == 1) ? (Print = true) : (Print = false);
    GlobCount = 0;
    NFact = fact(n);
    Ladder *l = newLadder(n);
    Ladder *ll = newLadder(n);
    initLadder(l);
    initLadder(ll);
    funcDriverTwo(l, n);
    SJTDriver(ll, n);
    destroyLadder(l);
    destroyLadder(ll);
}