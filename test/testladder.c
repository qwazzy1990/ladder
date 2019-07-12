#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ladder.h"
#include "utilities.h"

bool DEBUG1 = true;
bool DEBUG2 = false;


int main(int argc, char* argv[])
{
    int perm[4] = {4,3,2,1};
    int size = sizeof(perm)/sizeof(perm[0]);

    int permTwo[5] = {5,4,3,2,1};
    int sizeTwo = sizeof(permTwo)/sizeof(int);
    if(DEBUG1)
    {
        driver(perm, size);
    }
    if(DEBUG2)
    {
        driver(permTwo, sizeTwo);
    }


}