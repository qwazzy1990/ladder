//this algorithm generates all permutations of size N by creating cycles of permutations
//with 0...k inversions where K = N(N-1)/2. The maximal number of inversions for a permutation of size N.
//author: Patrick Di Salvo
//Date: July 10th 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "ladder.h"
#include "Sjt.h"

//for printing
bool Print = false;

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

void funcTwo(Ladder *l, int currLimit, int maxLimit, int currCol, int currRow, int *arr);

void removeAllBars(Ladder *l);
void __addBar(Ladder *l, int row, int col);
void removeSomeBars(Ladder *l, int currRow, int currCol);
void printLadderDisalvo(Ladder *l);
int calcOffset(Ladder *l);

int calcOffset(Ladder *l)
{

    //calculate the starting row
    int offset = l->numCols + 1;
    for (int i = 0; i < l->numCols; i++)
        offset++;

    return offset;
}

//prints the ladder
void printLadderDisalvo(Ladder *l)
{
    if (Print == false)
        return;
    int offset = calcOffset(l);
    for (int i = 0; i <= offset; i++)
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
    for (int i = 0; i < l->numCols + calcOffset(l)+1; i++)
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

void func(Ladder *l, int currLimit, int maxLimit, int currCol, int currRow, int *arr)
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

        globCount++;
        removeAllBars(l);
        return;
    }
    //if the current limit is at the max limit then return. This is the last ladder
    //in the set.
    if (currLimit > maxLimit)
        return;

    //if you are at the last column in the ladder
    if (currCol == l->numCols - 1)
    {
        int tempRow = currRow;
        //keep adding bars, bot right to top left until you get the the current limit
        for (int i = l->numCols - 1; i >= 0; i--)
        {
            __addBar(l, tempRow, i);
            //if you added the last bar then break
            if (l->numBars >= currLimit)
                break;

            tempRow--;
        }
        //if the number of bars = the current limit then print it
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
        //remove the bars just added and return
        removeSomeBars(l, currRow, currCol);
        return;
    }
    //if you are not at the last column then
    int tempRow = currRow;
    int tempCol = currCol;
    //go to the array at the current column
    for (int i = 0; i <= arr[currCol]; i++)
    {
        //make the recursive call if i = 0 then don't add any bars on the recursive call
        if (i == 0)
            func(l, currLimit, maxLimit, currCol + 1, currRow - 1, arr);
        //add a bar to the current column if i > 0
        else
        {
            __addBar(l, tempRow, tempCol);
            tempRow--;
            tempCol--;
            //if adding  this bar gets you to the current limit, then print it and break
            //and stop making recursive calls/adding bars
            if (l->numBars == currLimit)
            {
                if (Print)
                {
                    printf("Ladder Number: %d\n", globCount);
                    printLadderDisalvo(l);
                    printf("\n");
                }
                globCount++;

                break;
            }
            //make the recursive call with a new bar added
            func(l, currLimit, maxLimit, currCol + 1, currRow - 1, arr);
        }
    }
    //once you have added all the bars for the current column, from bot left to top right, remove them all
    removeSomeBars(l, currRow, currCol);
}

void funcTwo(Ladder *l, int currLimit, int maxLimit, int currCol, int currRow, int *arr)
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

        globCount++;
        removeAllBars(l);
        return;
    }

    //if the current limit is at the max limit then return. This is the last ladder
    //in the set.
    if (currLimit > maxLimit)
        return;

    if (currCol == 0)
    {
        int tempRow = currRow;
        for(int i = 0; i < l->numCols; i++)
        {
            __addBar(l, tempRow, i);
            tempRow++;
            if(l->numBars >= currLimit)break;
        }
          //if the number of bars = the current limit then print it
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
        //remove the bars just added and return
        removeSomeBars(l, currRow, currCol);
        return;
    }
    else
    {
        //if you are not at the last column then
        int tempRow = currRow;
        int tempCol = currCol;
        //go to the array at the current column
        for (int i = 0; i <= arr[currCol]; i++)
        {
            //make the recursive call if i = 0 then don't add any bars on the recursive call
            if (i == 0)
                funcTwo(l, currLimit, maxLimit, currCol - 1, currRow + 1, arr);
            //add a bar to the current column if i > 0
            else
            {
                __addBar(l, tempRow, tempCol);
                tempRow++;
                tempCol++;
                //if adding  this bar gets you to the current limit, then print it and break
                //and stop making recursive calls/adding bars
                if (l->numBars == currLimit)
                {
                    if (Print)
                    {
                        printf("Ladder Number: %d\n", globCount);
                        printLadderDisalvo(l);
                        printf("\n");
                    }
                    globCount++;

                    break;
                }
                //make the recursive call with a new bar added
                funcTwo(l, currLimit, maxLimit, currCol - 1, currRow + 1, arr);
            }
        }
        //once you have added all the bars for the current column, from bot left to top right, remove them all
        removeSomeBars(l, currRow, currCol);
    }
}

void funcDriver(Ladder *l, int maxLimit)
{
    //array used to keep track of the number of times needed to/
    //go through the loop for each column. Each index is a column for the ladder
    int *arr = calloc(l->numCols - 1, sizeof(int));
    int* arrTwo = calloc(l->numCols -1, sizeof(int));
    for (int i = 0; i < l->numCols - 1; i++){
        arr[i] = i + 1;

    }
    int c = l->numCols-1;
    for(int i = 0; i < l->numCols -1; i++)
    {
        arrTwo[c] = arr[i];
        c--; 
    }
    printPerm(arrTwo, l->numCols);

    //calculate the offset to determine the starting row
    int offset = calcOffset(l);
    for (int i = 0; i <= maxLimit; i++)
    {
        if(_DIRECTION_){
            func(l, i, maxLimit, 0, offset, arr);
            removeAllBars(l);
            _DIRECTION_ = !_DIRECTION_;
        }
        else 
        {
            funcTwo(l, i, maxLimit, l->numCols-1, 0, arrTwo);
            removeAllBars(l);

            _DIRECTION_ = !_DIRECTION_;
        }
    }
}

int main()
{
    int n;
    int p = 0;
    printf("Enter a number > 3\n");
    scanf("%d", &n);
    printf("Enter 1 to turn printing on\nEnter 0 to turn printing off\n");
    scanf("%d", &p);
    (p == 1) ? (Print = true) : (Print = false);
    Ladder *l = newLadder(n);
    initLadder(l);
    funcDriver(l, (n * (n - 1) / 2));
}