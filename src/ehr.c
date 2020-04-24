#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "ehr.h"
#include "ladder.h"
#include "utilities.h"
#include "Zaks.h"
void ehr(int** perms, int* perm, int n, int size)
{
    if(n == 2)
    {
        printPerm(perm, size);
        //appendPerm(perms, perm, size);
        swap(&(perm[0]), &(perm[1]), swapInts);
        printPerm(perm, size);
       // appendPerm(perms, perm, size);
        return;
    }
    forall(n)
    {
        //appendPerm(perms, perm, size);
        
        ehr(perms, perm, n-1, size);
        if(x != n-1)
            swap(&(perm[0]), &(perm[n-1]), swapInts);
        
       
        //appendPerm(perms, perm, size);
    }
}