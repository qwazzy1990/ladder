

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Sjt.h"
#include "ladder.h"
#include "utilities.h"
#include "Zaks.h"
#include "LinkedListAPI.h"


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

    for (int i = 0; i < n; i++)
    {
        if (i != 0 && dir[a[i] - 1] == 0)
        {
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

void printOnePerm(int** perms, int *a, bool *dir, int n, int count)
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

void genPermsSJT(List* perms, int *perm, int n, int *arr, bool *direction)
{

    //printPerm(perm, n);
    if(perms->length == 0)
        insertBack(perms, copyIntArr(perm, n));

    if (globalCount >= factorial(n)){
        globalCount = 0;
        return;
    }

    int p = -1;
    int q = -1;
    if (direction[n - 1] == LEFT)
    {
        p = n - 1;
        q = n - 2;
    }
    else
    {
        p = 0;
        q = 1;
    }
    forall(n - 1)
    {
        if (direction[n - 1] == LEFT)
        {
            _swap(&(perm[q]), &(perm[p]));
            p--;
            q--;
            globalCount++;
           
            arr[n - 1] += 1;
        }
        else
        {

            _swap(&(perm[p]), &(perm[q]));
            p++;
            q++;
            globalCount++;
           
            arr[n - 1] += 1;
        }
        //printPerm(perm, n);
        insertBack(perms, copyIntArr(perm, n));
    }

    direction[n - 1] = !(direction[n - 1]);
    adjustPerm(perms, perm, n - 1, arr, direction);
    globalCount++;
    genPermsSJT(perms, perm, n, arr, direction);
}

void adjustPerm(List* perms, int *perm, int n, int *arr, bool *direction)
{

    //globalCount++;
    int index = -1;
    for (int i = n-1; i >= 0; i--)
    {
    //     if you found the maximum n-1 value such that its bars have not been added or removed
    //     n-1 times
        if (arr[i] < i)
        {
           
            index = _getIndex(perm, n+1, i + 1);

            if (direction[i] == LEFT)
            {
                
                 _swap(&(perm[index - 1]), &(perm[index]));
                //add a bar to the ladder of level i+1
                //increment arr[i]
                arr[i] += 1;
            }
            else
            {
                 _swap(&(perm[index]), &(perm[index + 1]));
                
    //             add a bar to the ladder of level i+1
    //             increment arr[i]
                 arr[i] += 1;
             }
             insertBack(perms, copyIntArr(perm, n+1));
             //printPerm(perm, n+1);
             return;
        }
         //else set it to 0
         //change its direction
         else
         {
             arr[i] = 0;
             direction[i] = !(direction[i]);
         }
    }
    
}



void sjt(int *perm, int count, int max, int size, bool flag)
{
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

void copyPerm(int *dest, int *src, int n)
{
    forall(n)
    {
        dest[x] = src[x];
    }
}
//generates the permutations using the reverse of the sjt algorithm
void genPermsSJTReverse(int **perms, int *perm, int n, int *arr, bool *direction)
{
   
    //if the globalCount n! return
    if (globalCount == factorial(n))
        return;

    //pivot is an index for the direction and arr which contains the current direction and current count for element '1' in perm
    //
    int pivot = 0;
    //p and q are pointers to element 1 in perm and its next or previous neighbor in perm
    int p;
    int q;
    //if swapping righ then element 1 will be swapped with its right neighbor
    if (direction[pivot] == RIGHT)
    {
        p = 0;
        q = 1;
    }
    //else it will be swapped with its left neighbor
    else
    {
        p = n - 1;
        q = n - 2;
    }

    //perform n-2 swaps of element 1
    forall(n - 1)
    {
        if (direction[pivot] == RIGHT)
        {
            _swap(&(perm[p]), &(perm[q]));
            globalCount++;

            p++;
            q++;
        }
        else
        {
            _swap(&(perm[q]), &(perm[p]));
            globalCount++;

            p--;
            q--;
        }
    }

    //reverse the direction to be swapped for element 1
    direction[pivot] = !(direction[pivot]);
    //swap the next element  perm. The next element to be swapped ranges from 2 to N
    adjustPermReverse(perms, perm, n, arr, direction);
    //recursive call
    genPermsSJTReverse(perms, perm, n, arr, direction);
}

void adjustPermReverse(int **perms, int *perm, int n, int *arr, bool *direction)
{
    int index = -1;
    globalCount++;

    //for each of the next 2 to N elements
    for (int i = 1; i < n; i++)
    {
        //find the smallest element that can be swapped
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
            //if an element cannot be swapped reset it for later
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
void sjtLadder(Ladder* l, int n, int* arr, bool* direction)
{
    if(globalCount == factorial(n))
    {
        globalCount = 0;
        return;
    }
    forall(n-1)
    {
        printf("\nLadder Number = %d\n", globalCount + 1);
        //if the direction is true then add a bar
        if(direction[n-1] == false)
        {
            //column = (n-1) - i+1
            int col = (n-1) - (x+1);
            int row = col;
            int val1 = n;
            int val2 = (n) - (x+1);
            int up = -1;
          
                //swap them eventually
            //vals = n, n - (i+1)
            //need to get the bar number. Bar number = 
            printf("Adding a bar at level %d\n", n);
            printf("col:%d row:%d val1:%d val2:%d\n", col, row, val1, val2);
            //get th
        }
        else 
        {
            int col = x;
            int row = x;
            //step 1: get the bar number
            //step 2: get the bar
           

            printf("Removing a bar at level %d\n", n);
            printf("col:%d row:%d\n", col, row);
            //remove a bar belonging to lvl n to the ladder
        }
        globalCount++;
    }

    //readust the direction for the next call
    direction[n-1] = !(direction[n-1]);

    //call a function to add or remove a bar belnonging to level 1 <= bar <=n-1
    adjustLadder(l, arr, n-1, direction);
    globalCount++;

    sjtLadder(l, n, arr, direction);
}


//adds or removes a bar belonging to level 1 <= level <= n-1
void adjustLadder(Ladder* l, int* arr, int level, bool* direction)
{
    int count  = 0;
    int countTwo = 1;
    for(int i = level-1; i >= 0; i--)
    {
        //if you found the maximum n-i value such that its bars have not been added or removed 
        //n-i times
        if(arr[i] < i)
        {

            printf("\nLadder Number = %d\n", globalCount + 1);

            if(direction[i] == false)
            {
                //add a bar to the ladder of level i+1
                //increment arr[i]

                int row = (i+countTwo) - count - arr[i];
                int col = (i - 1 ) - arr[i];
                printf("Adding a bar at level %d\n", i+1);
                printf("Row:%d Col:%d\n", row, col);
                arr[i] += 1;
            }
            else 
            {
                //remove a bar from the ladder of i+1
                //incremement arr[i]
                //col = arr[i]
                //row = arr[i] + 2*count
                int col = arr[i];
                int row = arr[i] + 2*countTwo;
                printf("Removing a bar at level %d\n", i+1);
                printf("Row:%d Col:%d\n", row, col);
                arr[i] += 1;
            }
            return;
        }
        //else set it to 0
        //change its direction
        else 
        {
            printf("Resetting level %d\n", i+1);
            arr[i] = 0;
            direction[i] = !(direction[i]);
        }
        count--;
        countTwo++;
    }//end for
}


void reserveRows(int* reservations, int n)
{
    int count = n-2;
    for(int i = n; i> n - (n-2); i--)
    {
        reservations[i] = count;
        count = count + 1 + (i - 3);

    }
}

bool hasLeftNeighbor(Ladder* l, int row, int col)
{
    if(col == 0)return false;
    if(l->ladder[row][col-1] == 0)return false;
    return true;
}

bool hasRightNeighbor(Ladder* l, int row, int col)
{
    if(col == l->numCols) return false;
    if(l->ladder[row][col+1] == 0)return false;
    return true;
}
