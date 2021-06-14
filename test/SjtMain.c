

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include "ladder.h"
#include "utilities.h"
#include "Color.h"
#include "Sjt.h"

#include "LinkedListAPI.h"

bool _DEBUG1 = false;
bool _DEBUG2 = false;
bool _DEBUG3 = false;
bool DEBUG4 = true;

int _C_ = -1;
float xAxis = 0.0;
float yAxis = 15.0;

int yStart = 0;

int myPerm[13]= {1,2,3,4,5,6,7,8,9,10,11,12,13};

int rowCounter = 0;

int getInputGrayCode( void );

char* ladderToSvg(Ladder* l, int* perm);
void htmlHeader( void );
void htmlLadder(Ladder* l);
void htmlHeader( void )
{
    printf("<head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"></head>");
}
void htmlLadder(Ladder* l)
{
    char* s = calloc(1000, sizeof(char));
    int n = 2*(l->numCols) + 1;
   
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
     for (int i = 0; i <n; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            int val = l->ladder[i][j];
            if (val > 0)
            {
                s = realloc(s, memSize+10000);
                int x1 = gaps[j];
                int x2 = x1+40;
                int height = yStart + 2*i*rowHeight + 10;
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
    yStart+=110;
    printf("y start is %d\n", yStart);
    printf("%s", s);
    free(s);
}






int getInputGrayCode( void ){
    printf("Enter an n value\n");
    int nValue;
    fscanf(stdin, "%d", &nValue);
    return nValue;
}

int calcOffset(Ladder *l);

void __swap(int i, int j);
void __swap(int i, int j)
{
    int temp = myPerm[i]; 
    myPerm[i] = myPerm[j];
    myPerm[j] = temp;
}



int calcOffset(Ladder *l)
{
    int offset = l->numCols;
    offset = offset*2;
    return offset+1;

}




void printLadderDisalvo(Ladder *l);

void printLadderDisalvo(Ladder *l)
{
   
    int offset = calcOffset(l);
    printf("\n\n");
    for (int i = 0; i < offset; i++)
    {
        for (int j = 0; j < l->numCols; j++)
        {
            printf("|");
            if (l->ladder[i][j] == 1)
                printf("-----");
            else
                printf("     ");
        }
        printf("|");
        printf("\n");
    }
}

void sjtMod(Ladder *l, int count, int n, bool *dir);
void printLadderTikz(Ladder *l);
int calcDepth(Ladder* l);
int calcDepth(Ladder* l)
{
    int i = 0;
    int j = 0;
    bool flag = true;
    while(1)
    {
        for(int i = 0; i < l->numCols; i++)
        {
            if(l->ladder[j][i] == 1)
            {
                flag = false;
                break;
            }
        }
        if(flag == true)
            return j+1;
        else{
            flag = true;
            j++;
        }

    }
}



void printLadderTikz(Ladder *l)
{
    //    printf("\\begin{figure}[!htp]");
    //   printf("\\begin{tikzpicture}");
     _C_ ++;
    if(_C_ % 6 == 0)
    {
        xAxis = 0.0;
        yAxis -= 4;
    }

    float c1 = xAxis;
    float c2 = yAxis;
    for (int i = 0; i <= l->numCols; i++)
    {
        printf("\\draw(%.2f,%.2f) to (%.2f,%.2f);", c1, c2 -1.3, c1, c2 + 2);
        printf("\n");
        c1 += 0.4;
    }
    c1 = xAxis;
    c2 = c2 + 1.8;
    for (int i = 0; i <= 2*(l->numCols)-1; i++)
    {
         for (int j = 0; j < l->numCols; j++)
         {
             if (l->ladder[i][j] > 0)
             {
                 printf("\\draw(%.2f, %.2f) to (%.2f, %.2f);", c1, c2, c1 + .4, c2);
                 printf("\n");
             }
             c1 += .4;
         }
         c2 -= .6;
         c1 = xAxis;
    }

    c1 = xAxis;
    c2 = yAxis + 2.3;
    // c1 = *cA;
    forall(l->numCols+1)
    {
         printf("\\node at(%.2f,%.2f){\\tiny{%d}};", c1, (c2), myPerm[x]);
         printf("\n");
         c1 += .4;
    }

    //printf("\\end{tikzpicture}");
    //printf("\\end{figure}");
    xAxis += 2.0;
    //*cB += -1.5;
    printf("\n");
}


void sjtMod(Ladder *l, int count, int n, bool *dir)
{
    //if count > than max value=n
    if (count > n)
    {
        if(l->numBars == 0)
        {
           htmlLadder(l);
        }
        return;
    }

    for(int i = -1; i < count-1; i++)
    {
        //don't add or remove a bar on first iteration of the loop, just do recursive call
        if (i == -1)
            sjtMod(l, count + 1, n, dir);
        //if second or greater iteration then add or remove a bar
        else
        {
            //if adding a bar then go right to left
            if (dir[count - 1] == true)
            {
                int row = (n - 2) + (n - count) - (i);
                int col = (count - 2) - (i);
                l->ladder[row][col] = 1;
                l->numBars++;
               
               // __swap(myPerm[count-1], myPerm[count-2]);
            }
            //if removing a bar then go left to right
            else
            {
                int col = (i);
                int row = 2 * (n - count) + (i);
                l->ladder[row][col] = 0;
                l->numBars--;
                //__swap(myPerm[count-1], myPerm[count]);
            }
            //recursive call
            htmlLadder(l);
            sjtMod(l, count+1, n, dir);
        }
    }
    //switch the direction from right to left or left to right
    dir[count - 1] = !(dir[count - 1]);
}

int main()
{

    //gray code for generating ladders based on sjt
    if (_DEBUG1)
    {
        int n = 4;
        Ladder *l = newLadder(n);
        initLadder(l);

        int *reservedRows = calloc(n, sizeof(int));
        reserveRows(reservedRows, n);
        printf("%d\n", reservedRows[2]);

        for (int i = n - 3; i >= 0; i--)
        {
            //insertAt(l, n - 1, reservedRows, i);
        }

        for (int i = n - 2; i >= 0; i--)
        {
            // insertAt(l, n, reservedRows, i);
        }
        printf("%d %d %d %d\n", l->ladder[0][0], l->ladder[1][1], l->ladder[2][2], l->ladder[3][3]);

        destroyLadder(l);
        free(reservedRows);
    }

    //gray code for generating permutations based on sjt. Permutations used to generate ladders
    if (_DEBUG2)
    {
        int n = 0;
        scanf("%d", &n);
        new_object(int **, perms, fact(n) * 2);
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        int *perm = calloc(n, sizeof(int));
        forall(n)
        {
            direction[x] = RIGHT;
            arr[x] = 0;
            perm[x] = x + 1;
        }

        forall(fact(n) * 2)
            perms[x] = calloc(n, sizeof(int));

        //genPermsSJTReverse(perms, perm, n, arr, direction);

        for (int i = 0; i < fact(n); i++)
        {
            printf("    %d\n", i + 1);

            printPerm(perms[i], n);
            Ladder *l = newLadder(n);
            initLadder(l);
            createRoot(l, perms[i], n, 0);

            printLadder(l);
            destroyLadder(l);
        }
        for (int i = 0; i < fact(n) * 2; i++)
            free(perms[i]);

        free(perms);
        free(perm);
        free(arr);
        free(direction);
    }

    if (_DEBUG3)
    {
        int n = 0;
        scanf("%d", &n);
        new_object(int **, perms, fact(n) * 2);
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        int *perm = calloc(n, sizeof(int));
        forall(n)
        {
            direction[x] = LEFT;
            arr[x] = 0;
            perm[x] = x + 1;
        }
        forall(fact(n) * 2)
            perms[x] = calloc(n, sizeof(int));

        //genPermsSJT(perms, perm, n, arr, direction);
        for (int i = 0; i < fact(n); i++)
        {
            printf("    %d\n", i + 1);

            printPerm(perms[i], n);
            Ladder *l = newLadder(n);
            initLadder(l);
            createRoot(l, perms[i], n, 0);

            printLadder(l);
            destroyLadder(l);
        }
        for (int i = 0; i < fact(n) * 2; i++)
            free(perms[i]);

        free(perms);
        free(perm);
        free(arr);
        free(direction);
    }
    if (DEBUG4)
    {
        int n = getInputGrayCode();
        printf("n is %d", n);
        bool *dir = calloc(n, sizeof(bool));
        for (int i = 2; i <= n; i++)
        {
            dir[i - 1] = true;
        }
        Ladder *l = newLadder(n);
        initLadder(l);
        // to store the execution time of code
        double time_spent = 0.0;
 
        clock_t begin = clock();
 
    // do some stuff here
        htmlHeader();
        printf("<body>");
    
        printf("<svg height=%d width=%d/>", fact(n)*300, fact(n)*300);
      

        sjtMod(l, 2, n, dir);
        printf("</svg>");
        printf("</body>");
        clock_t end = clock();
    // calculate elapsed time by finding difference (end - begin) and
    // dividing the difference by CLOCKS_PER_SEC to convert to seconds
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
 
        
        printf("The elapsed time is %f seconds", time_spent);
       
    }
    return 0;
}
