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
    int perm[4] = {4, 3, 2, 1};
    int size = sizeof(perm)/sizeof(perm[0]);
    if(DEBUG1)
    {
        driver(perm, size);
    }
}