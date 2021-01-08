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
#include "svg.h"
#include "Zaks.h"
#include "Heaps.h"
#include "LinkedListAPI.h"
#include "ehr.h"
#include "lexicographic.h"
#include "GrayCode.h"
#include "HashMap.h"
//for the mid ladder problem
bool DEBUG0 = false;
bool DEBUG1 = false;
//for the enumeration problem
bool DEBUG7 = false;
//for the sjt problem
bool DEBUG8 = false;
//for counting degenerative triadic subsequences
bool DEBUG9 = false;
//for heaps ladders
bool DEBUG10 = false;
//for the gray code problem using all 4 algorithms
bool DEBUG11 = false;
bool DEBUG12 = true;
int memSize = 1;
int numDig = 0;

void getInput(int **perm, char *s);

void testDriverSJT(int n);

void runMinLadderExperiment(int start, int end, int numRuns, int mapSize);
char **readFile(FILE *fp);

void writeTexTable(char **s);

int kInversions(int *perm, int numDig);
int kInversions(int *perm, int numDig)
{
    int count = 0;
    for (int i = 0; i < numDig - 1; i++)
    {
        for (int j = i + 1; j < numDig; j++)
        {
            if (perm[i] > perm[j])
            {
                count++;
            }
        }
    }
    return count;
}

char *permToString(int *perm, int numDig)
{
    char *s = calloc(20, sizeof(char));
    forall(numDig)
    {
        char temp[10] = "";
        sprintf(temp, "%d ", perm[x]);
        strcat(s, temp);
    }
    return s;
}

void writeTexTable(char **s)
{

    FILE *fp = fopen("textTable.txt", "w");
    fprintf(fp, "\\begin{table} ");
    fprintf(fp, "\\begin{tabular}{ |p{5cm}||p{2cm}|p{2cm}|p{3cm}| }\\hline \\multicolumn{4}{|c|}{All OptL{$\\pi_{5}$} ordered by k inversions}\\\\ \\hline ");
    fprintf(fp, "Permutation & number & k & $|OptL{\\pi}|$\\\\ \\hline ");
    forall(120)
    {
        //get the
        int *perm = NULL;
        getInput(&perm, s[x]);
        int k = kInversions(perm, 5);
        char *s = permToString(perm, 5);
        saveAllLadders(perm, 5);
        fprintf(fp, "%s & ", s);
        fprintf(fp, "%d & ", x + 1);
        fprintf(fp, "%d & ", k);
        fprintf(fp, "%d\\\\ ", allLadders->length);
        numDig = 0;
        freeList(allLadders);
    }
    fprintf(fp, "\\hline ");
    fprintf(fp, "\\end{tabular} ");
    fprintf(fp, "\\label{table:KInvOptL} ");
    fprintf(fp, "\\end{table}");
    fclose(fp);
}

int main(int argc, char *argv[])
{
    PRINT = false;
    if (DEBUG7)
    {
        char *s = calloc(1000, sizeof(char));
        int *perm = NULL;

        printf("Enter a permutation\n");
        fgets(s, 1000, stdin);
        getInput(&perm, s);
        printf("Number of digits is %d\n", numDig);
        clear(s);

        printf("Input Permutation:\n");
        forall(numDig)
        {
            printf(GREEN " %d " COLOR_RESET, perm[x]);
        }
        printf("\n");
        saveAllLadders(perm, numDig);
        free(perm);
    }
    if (DEBUG8)
    {
        printf("Enter an integer greater than or equal to 1\n");
        int n;
        scanf("%d", &n);
        bool *direction = calloc(n, sizeof(bool));
        int *arr = calloc(n, sizeof(arr));
        new_object(int *, perm, n);
        forall(n)
        {
            direction[x] = false;
            arr[x] = 0;
            perm[x] = x + 1;
        }
        List *perms = initializeList(dummy_print, free, dummy_compare);
        genPermsSJT(perms, perm, n, arr, direction);
        //printAllPerms(perms, n);
        Node *node = perms->head;
        //printPerm(perms->head->data, n);
        Node *tempHead = perms->head;
        Ladder *l = newLadder(n);
        forall(n)
        {
            direction[x] = false;
            arr[x] = 0;
        }
        initLadder(l);
        sjtLadder(l, &node, n, arr, direction);
        perms->head = tempHead;
        destroyLadder(l);
        free(arr);
        free(direction);
        free(perm);
        freeList(perms);
    }

    if (DEBUG9)
    {
        int *perm = calloc(4, sizeof(int));
        forall(4)
        {
            perm[x] = 4 - x;
        }

        int n = countDegenerativeSubsequences(perm, 4);
        new_object(DTSA *, dts, n);
        setDTSs(dts, perm, n, 4);

        freeDts(dts, n);
        free(perm);
    }

    if (DEBUG10)
    {
        int n = 4;
        List *l = initializeList(dummy_print, free, dummy_compare);
        new_object(int *, perm, 4);
        forall(n) perm[x] = x + 1;

        heaps(l, perm, n, n - 1);
        printAllPerms(l, n);
        freeList(l);
        free(perm);
    }
    if (DEBUG11)
    {
        int n = 7;

        grayCodeDriver(n);
    }
    /**
     * For creating a LaTex table with the size of OptL{pi} for each 
     * pi of order 5. The permutations are ordered by the number of inversions from 0 to choose(5, 2)
     * */
    if (DEBUG12)
    {
        FILE *fp = fopen("KInversions.txt", "r");
        char **s = readFile(fp);
        fclose(fp);
        writeTexTable(s);
    }
}

void getInput(int **perm, char *s)
{
    *perm = calloc(memSize, sizeof(int));
    char delim[5];
    strcpy(delim, " \n");
    char *token = NULL;
    token = strtok(s, delim);
    int muls = 1;

    while (token != NULL)
    {

        int currNum = 0;
        for (int i = strlen(token) - 1; i >= 0; i--)
        {
            currNum += muls * (token[i] - 0x30);
            muls *= 10;
        }

        (*perm)[numDig] = currNum;

        numDig++;
        memSize++;
        muls = 1;

        token = strtok(NULL, delim);
        *perm = (int *)realloc(*perm, memSize * sizeof(int));
    }
    // forall(strlen(s))
    // {
    //     if (isdigit(s[x]))
    //     {
    //         (*perm)[numDig] = s[x] - 0x30;
    //         numDig++;
    //         memSize++;
    //         *perm = (int *)realloc(*perm, memSize * sizeof(int));
    //     }
    // }
}

char **readFile(FILE *fp)
{
    char **s = calloc(200, sizeof(char *));
    forall(200)
    {
        s[x] = calloc(20, 1);
    }
    int row = 0;
    int col = 0;
    while (!feof(fp))
    {
        char c = fgetc(fp);
        if (c == '\n')
        {
            row++;
            col = 0;
        }
        else
        {
            s[row][col] = c;
            col++;
        }
    }
    return s;
}
