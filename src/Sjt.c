

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Sjt.h"
#include "ladder.h"
#include "utilities.h"

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
         printf("%d ", a[i]);
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

void printPerms(int** perms, int n)
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
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
        //perms[0][i] = a[i];
    }
    printf("   %d\n", 1);

    int count = 1;
    for (int i = 0; i < fact(n) - 1; i++)
    {
        //perms[count] = 
        printOnePerm(perms, a, dir, n, count);
        printf("   %d\n", i + 2);
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
