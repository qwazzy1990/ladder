#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "ladder.h"
#include "utilities.h"
#include "LinkedListAPI.h"
#include "HashMap.h"
#include "Color.h"

//flags for the main
bool DEBUG0 = false;
bool DEBUG1 = false;
bool DEBUG2 = true;
bool DEBUGN = false;

//end flags

void printLadderDisalvo(Ladder *l);


List *uninvert(int *perm, int n);
List *copyList(List *og, int nn);
int getCount(int *perm, int start, int n);
void swapEven(int *perm, int start, int end, int n);
void getInput(int **perm, char *s);
void swapOdd(int *perm, List *perms, int start, int end, int n);
void preProcessRowZero(Ladder *l, int *og, int *cpy, int n);
int funcGetCount(int *perm, int start, int *end, int n);
void swapDSS(Ladder *l, int *perm, int start, int count, int currRow);
void createMinLadder(Ladder *l, int *perm, int n, int currRow);

int *generateRandomPerm(int n, HashMap *h);
Ladder *minLadderDriver(int *perm, int n);

int T(int n);

double TFormula(int n);

void genT(Ladder* l, int n);
int getMaxLenSubString(int *perm, int n);
int max(int a, int b);
int numDig = 0;

static int *copyPerm(int *og, int n)
{
    new_object(int *, cpy, n);
    forall(n) cpy[x] = og[x];
    return cpy;
}

int max(int a, int b)
{
    if (a >= b)
        return a;
    return b;
}
List *copyList(List *og, int nn)
{
    List *cpy = initializeList(og->printData, og->deleteData, og->compare);
    for (Node *n = og->head; n != NULL; n = n->next)
    {
        int *perm = n->data;
        insertBack(cpy, copyPerm(perm, nn));
    }
    return cpy;
}
List *uninvert(int *perm, int n)
{
    new_object(int *, starts, n);
    new_object(int *, ends, n);

    forall(n) starts[x] = ends[x] = 0;

    int c = 0;
    for (int i = 0; i < n; i++)
    {
        //if there is an inversion
        if (i == n - 1)
            break;
        if (perm[i] > perm[i + 1])
        {
            int count = getCount(perm, i, n);
            starts[c] = i;
            ends[c] = i + count;
            i += count;
            c++;
        }
    }
    c = 0;
    int oddCount = 0;
    while (ends[c] != 0)
    {
        //swap all the evens
        if ((ends[c] - starts[c]) % 2 == 1)
        {
            swapEven(perm, starts[c], ends[c], n);
        }
        //add to the odd count
        else
        {
            oddCount++;
        }
        c++;
    }
    c = 0;
    int y = 0;
    //holds all the odd length adj. inversions
    List *oddList = initializeList(dummy_print, dummy_delete, dummy_compare);

    //if there are any odd lengthed inversions
    if (oddCount > 0)
    {
        while (ends[c] != 0)
        {
            //swap the first odd lengthed DSS in all possible ways
            if ((ends[c] - starts[c]) % 2 == 0 && y == 0)
            {

                int *cpy = copyPerm(perm, n);
                List *l = initializeList(dummy_print, dummy_delete, dummy_compare);
                swapOdd(cpy, l, starts[c], ends[c], n);
                free(cpy);
                insertBack(oddList, l);
                y++;
            }
            //add the Nth odd lenthed DSS in all possible ways and concatenate them to all the previoously swapped DSS odds
            else if ((ends[c] - starts[c]) % 2 == 0 && y > 0)
            {
                List *l = getFromBack(oddList);
                List *newL = initializeList(dummy_print, dummy_delete, dummy_compare);
                for (Node *nn = l->head; nn != NULL; nn = nn->next)
                {
                    int *cpy = copyPerm(nn->data, n);
                    swapOdd(cpy, newL, starts[c], ends[c], n);
                }
                insertBack(oddList, newL);
            }

            c++;
        }
    }

    else
    {
        List *ll = initializeList(dummy_print, free, dummy_compare);
        insertBack(ll, copyPerm(perm, n));
        free(starts);
        free(ends);
        freeList(oddList);
        return ll;
    }
    free(starts);
    free(ends);
    List *finList = copyList(getFromBack(oddList), n);
    freeList(oddList);
    return finList;
}

void preProcessRowZero(Ladder *l, int *og, int *cpy, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (og[i] != cpy[i])
        {
            addBar(l, og[i], og[i + 1], 0, i);
            i++;
        }
    }
}

int funcGetCount(int *perm, int start, int *end, int n)
{
    int count = 1;
    while (perm[start] > perm[start + 1])
    {

        start++;
        count++;
        if (start == n - 1)
            break;
    }
    *end = start;
    return count;
}

void swapDSS(Ladder *l, int *perm, int start, int count, int currRow)
{

    for (int i = 0; i < count / 2; i++)
    {
        addBar(l, perm[start], perm[start + 1], currRow, start);

        swapInts(&(perm[start]), &(perm[start + 1]));
        start += 2;
    }
}

void createMinLadder(Ladder *l, int *perm, int n, int currRow)
{

    if (isSorted(perm, n))
        return;

    int end;
    int count;
    forall(n - 1)
    {
        if (perm[x] > perm[x + 1])
        {
            count = funcGetCount(perm, x, &end, n);
            if (!(count % 2))
            {
                swapDSS(l, perm, x, count, currRow);
            }
            else
            {
                addBar(l, perm[x + 1], perm[x + 2], currRow, x + 1);
                swapInts(&(perm[x + 1]), &(perm[x + 2]));
                swapDSS(l, perm, x + 3, count - 3, currRow);
            }
            x = end;
        }
    }
    createMinLadder(l, perm, n, currRow + 1);
    l->depth++;
}

Ladder *minLadderDriver(int *perm, int n)
{
    int *og = copyPerm(perm, n);
    List *swappedLists = uninvert(perm, n);
    int bestScore = 10000;
    int *bestPerm = NULL;
    Ladder *l = newLadder(n);
    initLadder(l);

    for (Node *nn = swappedLists->head; nn != NULL; nn = nn->next)
    {
        int maxLen = getMaxLenSubString(nn->data, n);
        if (maxLen < bestScore)
        {
            bestScore = maxLen;
            bestPerm = nn->data;
        }
    }
    preProcessRowZero(l, og, bestPerm, n);
    createMinLadder(l, bestPerm, n, 1);
    free(og);
    freeList(swappedLists);
    //printLadder(l);
    return l;
}

void runMinLadderExperiment(int start, int end, int numRuns, int mapSize)
{
    HashMap *hMap = newMap(1000, dummy_print, &free, &strcmp);
    for (int i = start; i <= end; i++)
    {
        for (int j = 0; j < numRuns; j++)
        {

            int *perm = generateRandomPerm(i, hMap);

            printf("\nInput Permutation:\n");
            forall(i)
            {
                printf(" %d ", perm[x]);
            }
            printf("\n");

            genMinLadders(perm, i);
            if (PRINT)
            {
                printf("\n-- GENERATING ALL MIN LADDERS----\n");
                printLadder(getFromBack(minLadders));
                printf("\n--END OF GENERATING ALL MIN LADDERS---\n");
                PRINT = true;
            }
            Ladder *l = minLadderDriver(perm, i);
            if (PRINT)
            {
                printf("\n---MINIMUM LADDER CREATED----\n");
                printLadder(l);

                printf("\n---END OF MINIMUM LADDER CREATED---\n");
                PRINT = true;
            }
            Ladder *ll = getFromBack(minLadders);
            printf("Brute Force: %d  Custom: %d\n", ll->depth + 1, l->depth + 1);
            free(perm);
            freeList(minLadders);
            destroyLadder(l);
        }
    }
    deleteHashMap(hMap);
}
int *generateRandomPerm(int n, HashMap *h)
{
    new_object(int *, perm, n);
    forall(n) perm[x] = 0;
    int numDig = 0;
    time_t t;
    bool repeat = false;

    /* Intializes random number generator */
    srand((unsigned)time(&t));

    while (1)
    {
        int randDig = (rand() % n) + 1;
        for (int i = 0; i < numDig; i++)
        {
            if (perm[i] == randDig)
            {
                repeat = true;
                break;
            }
        }
        if (!repeat)
        {
            perm[numDig] = randDig;
            numDig++;
        }
        else
        {
            repeat = !repeat;
        }
        if (numDig == n)
        {
            char *s = calloc(n + 2, sizeof(char));
            forall(n)
            {
                s[x] = perm[x] + '0';
            }
            if (containsData(h, s) == false)
            {
                insertToMap(h, s);
                return perm;
            }
            else
            {
                free(perm);
                free(s);
                perm = calloc(n, sizeof(int));
                numDig = 0;
            }
        }
    }
}

void swapOdd(int *perm, List *perms, int start, int end, int n)
{
    if (end - start == 2)
    {
        int *c1 = copyPerm(perm, n);
        int *c2 = copyPerm(perm, n);
        swapInts(&(c1)[start], &(c1)[start + 1]);
        swapInts(&(c2)[start + 1], &(c2)[end]);
        insertBack(perms, c1);
        insertBack(perms, c2);
        return;
    }
    else if (end - start == 1)
    {
        int *c1 = copyPerm(perm, n);
        swapInts(&(c1)[start], &(c1)[start + 1]);
        insertBack(perms, c1);
        return;
    }
    else if (start >= end)
        return;
    else
    {
        //swap first two

        int *c1 = copyPerm(perm, n);
        int *c2 = copyPerm(perm, n);
        swapInts(&(c1)[start], &(c1)[start + 1]);
        swapInts(&(c2)[start + 1], &(c2)[start + 2]);
        swapOdd(c1, perms, start + 2, end, n);

        swapOdd(c2, perms, start + 3, end, n);
        free(c1);
        free(c2);
    }
}

void swapEven(int *perm, int start, int end, int n)
{
    for (int i = start; i <= end; i += 2)
    {
        swapInts(&(perm[i]), &(perm[i + 1]));
    }
}
int getCount(int *perm, int start, int n)
{
    int count = 0;
    while (perm[start] > perm[start + 1])
    {
        count++;
        start++;
        if (start == n - 1)
            break;
    }
    return count;
}

int getMaxLenSubString(int *perm, int n)
{
    int maxIncrease = 0;
    int maxDecrease = 0;
    int len = 0;
    for (int i = 0; i < n; i++)
    {
        if (i == n - 1)
            break;
        if (perm[i] > perm[i + 1])
        {
            len++;
        }
        else
        {
            if (len > maxDecrease)
            {
                maxDecrease = len;
            }
            len = 0;
        }
    }
    len = 0;
    for (int i = 0; i < n; i++)
    {
        if (i == n - 1)
            break;
        if (perm[i] < perm[i + 1])
        {
            len++;
        }
        else
        {
            if (len > maxIncrease)
            {
                maxIncrease = len;
            }
            len = 0;
        }
    }
    return max(maxIncrease, maxDecrease);
}

void getInput(int **perm, char *s)
{
    *perm = calloc(1, sizeof(int));
    char delim[5];
    strcpy(delim, " \n");
    char *token = NULL;
    token = strtok(s, delim);
    int muls = 1;
    int memSize = 1;

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
}

int T(int n)
{
   if(n == 0)return 0;
   if(n == 1)return 0;
   int count = T(n-1) + T(n-2) + 1;
   return count; 
}

double TFormula(int n)
{
    double a = 1/sqrt(5);
    double b = pow((1+sqrt(5))/2, n+1);
    double c = pow((1-sqrt(5))/2, n+1);

    double d = 5 + (3*sqrt(5));
    double e = 5 - (3*sqrt(5));
    double f = (1 + sqrt(5))/2; 
    double g = (1 - sqrt(5))/2;

    double h = (d*pow(e, n));
    double i = (f*pow(g, n));
    double j = ((h + i)/10) - 1;
    printf("---j is %f----\n", j);


    return (a*(b-c))-1;
}

void genT(Ladder* l, int n)
{
    if(n < 0)return;
   
    l->ladder[1][n] = 1;
        printf("\n");
        printLadderDisalvo(l);
        printf("\n");
    genT(l, n-2);
    l->ladder[1][n] = 0;
    genT(l, n-1);
    
}
//prints the ladder with bars and columns
void printLadderDisalvo(Ladder *l)
{
    
    int offset = 3;
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
int main()
{
    if (DEBUG0)
    {
        char *s = calloc(1000, sizeof(char));
        int *perm = NULL;

        printf("Enter a permutation\n");
        fgets(s, 1000, stdin);
        getInput(&perm, s);
        clear(s);
        int *p2 = copyPerm(perm, numDig);

        Ladder *l = minLadderDriver(perm, numDig);
        genMinLadders(p2, numDig);
        destroyLadder(l);
    }
    if (DEBUG1)
    {
        int start = 0;
        int end = 0;
        int numRuns = 0;
        unsigned long hashSize = 0;
        printf("Enter the starting N value\n");
        scanf("%d", &start);
        printf("Enter the ending N value\n");
        scanf("%d", &end);
        printf("Enter the number of runs\n");
        scanf("%d", &numRuns);
        printf("Enter the size of the hash map\n");
        scanf("%lu", &hashSize);
        PRINT = false;
        runMinLadderExperiment(start, end, numRuns, hashSize);
    }
    if (DEBUG2)
    {
        for(int i = 2; i <= 15; i++){
            printf("%d %d %d\n", i, (int)TFormula(i), T(i));
        }
    }
    if(DEBUGN)
    {
        int n;
        printf("Enter the number of columns for the ladder\n");
        scanf("%d", &n);
        Ladder* l = newLadder(n+1);
        initLadder(l);
        genT(l, n-1);
    }

    return 0;
}
