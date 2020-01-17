///test

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

char* htmlLadder(Ladder* l, int yStart)
{
    char* s = calloc(1000, sizeof(char));
   
    int memSize = 1000;
    int cols = 20;
    int* gaps = calloc(l->numCols+1, sizeof(int));

    forall(l->numCols+1)
    {
        memSize += 1000;
        s = realloc(s, memSize*sizeof(char));
        char* temp = calloc(50, sizeof(char));
        gaps[x] = cols;
        sprintf(temp, "%d", cols);
        strcat(s, "<line x1=\"");
        strcat(s, temp);
        free(temp);
        strcat(s, "\" ");
        temp = calloc(50, sizeof(char));
        sprintf(temp, "%d", yStart);
        strcat(s, "y1=\"");
        strcat(s, temp);
        strcat(s, "\" ");
        free(temp);
        strcat(s, "x2=\"");
        temp = calloc(50, sizeof(char));
        sprintf(temp, "%d", cols);
        strcat(s, temp);
        free(temp);
        strcat(s, "\" ");
        temp = calloc(50, sizeof(char));
        sprintf(temp,"%d", yStart + 100);
        strcat(s, "y2=\"");
        strcat(s, temp);
        strcat(s, "\" ");
        strcat(s, "style=\"stroke:rgb(255,0,0);stroke-width:2\"/>");
        free(temp);
        cols+=40;
    }


    int rowHeight = 5;
     for (int i = 0; i <= l->depth + 1; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val != 0)
            {
                s = realloc(s, memSize+10000);
                int x1 = gaps[j];
                int x2 = x1+40;
                int height = yStart + i*rowHeight + 10;
                char* temp = calloc(50, sizeof(char));
                sprintf(temp, "%d", x1);
                strcat(s, "<line x1=\"");
                strcat(s, temp);
                free(temp);
                strcat(s, "\" ");
                strcat(s, "y1=\"");
                temp = calloc(50, sizeof(char));
                sprintf(temp, "%d", height);
                strcat(s, temp);
                free(temp);
                strcat(s, "\" ");
                strcat(s, "x2=\"");
                temp = calloc(50, sizeof(char));
                sprintf(temp, "%d", x2);
                strcat(s, temp);
                free(temp);
                strcat(s, "\" ");
                strcat(s, "y2=\"");
                temp = calloc(50, sizeof(char));
                sprintf(temp, "%d", height);
                strcat(s, temp);
                free(temp);
                strcat(s, "\" ");
                strcat(s, "style=\"stroke:rgb(255,0,0);stroke-width:2\"/>");
                memSize += 500;



            }
        }
        
    }

    return s;
}



char* htmlBody(List* list)
{

    int memSize = 1000;
    char* s = calloc(memSize, sizeof(char));
    char* head = htmlHeader();
    strcat(s, head);
    free(head);
    strcat(s, "<body>");
    ListIterator itr = createIterator(list);
    Ladder* l = NULL;
    int i = 0;
    char* temp = calloc(100, sizeof(char));
    sprintf(temp, "%d", list->length*300);
    printf("Number of ladders is %d\n", list->length);
    strcat(s, "<svg height=");
    strcat(s, temp);
    strcat(s, "\" ");
    strcat(s, "width=");
    strcat(s, temp);
    strcat(s, "\">");
    free(temp);

    while((l = nextElement(&itr))!= NULL)
    {
        char* temp = htmlLadder(l, i);
        memSize += strlen(temp);
        s = realloc(s, memSize*sizeof(char));
        strcat(s, temp);
        free(temp);
        i+= 110;
    }
    strcat(s, "</svg>");

    strcat(s, "</body>");
    return s;

}
