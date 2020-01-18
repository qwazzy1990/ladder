

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Sjt.h"
#include "ladder.h"
#include "utilities.h"

int globalCount = 0;
int searchArr(int *a, int n, int mobile)
{
    for (int i = 0; i < n; i++)
        if (a[i] == mobile)
            return i + 1;

    return 0;
}

int getMobile(int *a, bool *dir, int n)
{
    int prevMobile = 0;
    int mobile = 0;

    //remember that this was originally for i = 0; i < n; i++
    for (int i = 0; i < n; i++)
    {
        if (i != 0 && dir[a[i] - 1] == 0)
        {
            //remember that originally these were greater tha
            //less that just for testing purposes
            if (a[i] > a[i - 1] && a[i] > prevMobile)
            {
                mobile = a[i];
                prevMobile = mobile;
            }
        }

        if (i != n - 1 && dir[a[i] - 1] == 1)
        {
            if (a[i] > a[i + 1] && a[i] > prevMobile)
            {
                mobile = a[i];
                prevMobile = mobile;
            }
        }
    }

    if (mobile == 0 && prevMobile == 0)
        return 0;
    return mobile;
}

void printOnePerm(int **perms, int *a, bool *dir, int n, int count)
{
    int mobile = getMobile(a, dir, n);
    int pos = searchArr(a, n, mobile);

    // swapping the elements
    // according to the
    // direction i.e. dir[].
    if (dir[a[pos - 1] - 1] == 0)
    {
        int temp = a[pos - 1];
        a[pos - 1] = a[pos - 2];
        a[pos - 2] = temp;
    }

    else if (dir[a[pos - 1] - 1] == 1)
    {
        int temp = a[pos];
        a[pos] = a[pos - 1];
        a[pos - 1] = temp;
    }

    // changing the directions
    // for elements greater
    // than largest mobile integer.
    for (int i = 0; i < n; i++)
    {
        if (a[i] > mobile)
        {
            if (dir[a[i] - 1] == 0)
                dir[a[i] - 1] = 1;

            else if (dir[a[i] - 1] == 1)
                dir[a[i] - 1] = 0;
        }
    }

    perms[count] = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
    {
        //printf("%d ", a[i]);
        perms[count][i] = a[i];
    }
}

int fact(int n)
{
    int res = 1;
    for (int i = 1; i <= n; i++)
    {
        res = res * i;
    }
    return res;
}

void genPermsSJT(int **perms, int n)
{

    int *a = calloc(n, sizeof(int));
    bool *dir = calloc(n, sizeof(bool));
    perms[0] = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
    {
        a[i] = i + 1;
        dir[i] = 0;
        perms[0][i] = a[i];
    }
    //perms[0] = calloc(n, sizeof(int));

    printf("   %d\n", 1);

    int count = 1;
    for (int i = 0; i < fact(n) - 1; i++)
    {
        //perms[count] =
        printOnePerm(perms, a, dir, n, count);
        count++;
    }
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

void genPermsSJTReverse(int** perms, int *perm, int n, int *arr, bool *direction)
{
    printPerm(perm, n);
    if (globalCount == factorial(n))
        return;

    int pivot = 0;
    int p;
    int q;
    if (direction[pivot] == RIGHT)
    {
        p = 0;
        q = 1;
    }
    else
    {
        p = n - 1;
        q = n - 2;
    }
    forall(n - 1)
    {
        if (direction[pivot] == RIGHT)
        {
            _swap(&(perm[p]), &(perm[q]));
            printPerm(perm, n);
            p++;
            q++;
            globalCount++;
        }
        else
        {
            _swap(&(perm[q]), &(perm[p]));
            printPerm(perm, n);

            p--;
            q--;
            globalCount++;
        }
    }
    direction[pivot] = !(direction[pivot]);
    adjustPerm(perm, n, arr, direction);
    genPermsSJTReverse(perms, perm, n, arr, direction);
}

void adjustPerm(int *perm, int n, int *arr, bool *direction)
{
    int index = -1;
    globalCount++;

    for (int i = 1; i < n; i++)
    {
        if (arr[i] < n - (i + 1))
        {
            if (direction[i] == RIGHT)
            {
                index = _getIndex(perm, n, i + 1);
                _swap(&(perm[index]), &(perm[index + 1]));
                arr[i] += 1;
            }
            else
            {
                index = _getIndex(perm, n, i + 1);
                _swap(&(perm[index]), &(perm[index - 1]));
                arr[i] += 1;
            }

            return;
        }
        else
        {
            arr[i] = 0;
            direction[i] = !(direction[i]);
        }
    }
}

void _swap(int *v1, int *v2)
{
    int temp1 = *v1;
    *v1 = *v2;
    *v2 = temp1;
}

int _getIndex(int *perm, int n, int val)
{
    forall(n)
    {
        if (perm[x] == val)
            return x;
    }
    return -1;
}
//Sjt algorithm for generating ladders
void sjtLadder(Ladder *l, int n, int *arr, bool *direction)
{
    if (direction[1] == false && arr[1] == 0)
        return;
    forall(n - 1)
    {
        printf("\nLadder Number = %d\n", globalCount + 2);
        globalCount += 1;
        //if the direction is true then add a bar
        if (direction[n - 1] == true)
        {
            printf("Adding a bar at level %d\n", n);
        }
        else
        {
            printf("Removing a bar at level %d\n", n);
            //remove a bar belonging to lvl n to the ladder
        }
    }

    //readust the direction for the next call
    direction[n - 1] = !(direction[n - 1]);

    //call a function to add or remove a bar belnonging to level 1 <= bar <=n-1
    adjustLadder(l, arr, n - 1, direction);

    sjtLadder(l, n, arr, direction);
}

//adds or removes a bar belonging to level 1 <= level <= n-1
void adjustLadder(Ladder *l, int *arr, int level, bool *direction)
{
    for (int i = level - 1; i >= 0; i--)
    {
        //if you found the maximum n-1 value such that its bars have not been added or removed
        //n-1 times
        if (arr[i] < i)
        {
            printf("\nLadder Number = %d\n", globalCount + 2);
            globalCount++;

            if (direction[i] == true)
            {
                //add a bar to the ladder of level i+1
                //increment arr[i]
                printf("Adding a bar at level %d\n", i + 1);
                arr[i] += 1;
            }
            else
            {
                //remove a bar from the ladder of i+1
                //incremement arr[i]
                printf("Removing a bar at level %d\n", i + 1);
                arr[i] += 1;
            }
            return;
        }
        //else set it to 0
        //change its direction
        else
        {
            printf("Resetting level %d\n", i + 1);
            arr[i] = 0;
            direction[i] = !(direction[i]);
        }
    }
}

//reserves a number of rows for each val from n to 2
void reserveRows(int *reservations, int n)
{
    int count = n - 2;
    for (int i = n; i >= n - (n - 2); i--)
    {
        reservations[i] = count;
        count = count + 1 + (i - 3);
    }
}

//inserts  a bar at the row within the range of reserved rows based on the val.
//the column is there
void insertAt(Ladder *l, int val, int *reserved, int col)
{
    int startRow = reserved[val];

    //find the first empty row for the val
    //since we always add right to left it will be equal to the
    while (emptyRow(l, startRow) == false)
    {
        startRow--;
    }

    if (hasLowerNeighbor(l, val, startRow, col, reserved[2]))
    {
        printf("Has a lower neighbor\n");
    }
    if (hasUpperNeighbor(l, val, startRow, col, 0))
    {
        printf("Has an upper neigbor\n");
    }
    l->ladder[startRow][col] = 1;
}

bool hasLeftNeighbor(Ladder *l, int row, int col)
{
    if (col == 0)
        return false;
    if (l->ladder[row][col - 1] == 0)
        return false;
    return true;
}

bool hasRightNeighbor(Ladder *l, int row, int col)
{
    if (col == l->numCols - 1)
        return false;
    if (l->ladder[row][col + 1] == 0)
        return false;
    return true;
}

//upper bound will be equal to row 0
bool hasUpperNeighbor(Ladder *l, int n, int row, int col, int upperBound)
{

    if (col == l->numCols - 1)
        return false;
    for (int i = row - 1; i >= upperBound; i--)
    {
        if (l->ladder[i][col + 1] == 1)
            return true;
    }

    return false;
}

//checks if there is a lower neighbor of the palce where the current bar will go
//lower bound will be equal to the last place a bar can go whcich is at reservedVals[2]
bool hasLowerNeighbor(Ladder *l, int n, int row, int col, int lowerBound)
{
    if (col == 0)
        return false;
    for (int i = row + 1; i <= lowerBound; i++)
    {
        if (l->ladder[i][col - 1] == 1)
            return true;
    }

    return false;
}
