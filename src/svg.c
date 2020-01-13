#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ladder.h"
#include "utilities.h"


char* ladderToSvg(Ladder* l, int* perm);

char* htmlHeader( void )
{
    char* header = calloc(1000, sizeof(char));
    strcpy(header, "<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"></head>");
    return header;
}

char* htmlBody(Ladder* l, int* perm)
{
    char* s = calloc(10000, sizeof(char));
    int memSize = 500;
    char* head = htmlHeader();
    strcat(s, head);
    free(head);
    strcat(s, "<body>");
    strcat(s, "<svg height=\"200\" width=\"200\">");
    int cols = 20;
    forall(l->numCols+1)
    {
        s = realloc(s, memSize+10000);
        char* temp = calloc(50, sizeof(char));
        sprintf(temp, "%d", cols);
        strcat(s, "<line x1=\"");
        strcat(s, temp);
        strcat(s, "\" ");
        strcat(s, "y1=\"0\" ");
        strcat(s, "x2=\"");
        strcat(s, temp);
        strcat(s, "\" ");
        strcat(s, "y2=\"100\" ");
        strcat(s, "style=\"stroke:rgb(255,0,0);stroke-width:2\"/>");
        free(temp);
        memSize += 500;
        cols+=40;
    }
    strcat(s, "</svg>");

    strcat(s, "</body>");
    return s;
}