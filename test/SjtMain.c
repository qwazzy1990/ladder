

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"
#include "Sjt.h"
#include "svg.h"
#include "LinkedListAPI.h"

int main()
{

   // Ladder* l = newLadder(5);
   // initLadder(l);

    int* reservedRows = calloc(5, sizeof(int));
    reserveRows(reservedRows, 5);
    free(reservedRows);

    return 0;
}