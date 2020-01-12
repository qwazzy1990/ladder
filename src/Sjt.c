

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "Sjt.h"

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
