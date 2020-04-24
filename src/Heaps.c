#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Heaps.h"
#include "ladder.h"
#include "Zaks.h"
#include "utilities.h"
#include "LinkedListAPI.h"

void heaps(List *perms, int *perm, int n, int idx)
{
    if (idx == 0)
    {
        //appendPerm(perms, perm, n);
        if(perms->length == 0)
            insertBack(perms, copyIntArr(perm, n));
        return;
    }
    else
    {

        forall(idx)
        {
            heaps(perms, perm, n, idx - 1);
            if (idx % 2 == 0)
            {
                int temp = perm[idx];
                perm[idx] = perm[0];
                perm[0] = temp;
            }
            else
            {
                int temp = perm[idx];
                perm[idx] = perm[x];
                perm[x] = temp;
            }
            insertBack(perms, copyIntArr(perm, n));
            //printPerm(perm, n);
            
        }
        if (idx > 1)
        {
            heaps(perms, perm, n, idx - 1);

        }
    }
}