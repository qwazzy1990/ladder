#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Heaps.h"
#include "ladder.h"
#include "Zaks.h"
#include "utilities.h"


void heaps(int** perms, int* perm, int n, int idx)
{
    if(idx == 1)
    {
        appendPerm(perms, perm, n);
        return;
    }
    else{
        
        forall(idx)
        {
            heaps(perms, perm, n, idx-1);
            if(idx %2 == 0)
            {
                int temp = perm[x];
                perm[x] = perm[idx-1];
                perm[idx-1] = temp;
                //appendPerm(perms, perm, n);

            }
            else 
            {
                int temp = perm[0];
                perm[0] = perm[idx-1];
                perm[idx-1] = temp;
                //appendPerm(perms, perm, n);
            }
           
            
            
        }
    }
}