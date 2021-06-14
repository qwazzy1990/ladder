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

void walsh(int *p, int *inverse, int pivot1, int pivot2, bool down);

void printPerm(int *p, int n);

void setPivots(int *g, int *m, int *pivot1, int *pivot2, int N, int r);

int max(int *g, int *m, int idx, int r, int N);

int min(int *g, int *m, int idx, int r, int N);

int mobileMax(int* g, int idx, int r, int N);

int mobileMin(int* g, int idx, int r, int N);

int *inverse(int *p, int N);


bool parity(int *g, int idx, int N);

void nextLadder(bool* increase, int* g, int* m, int, int w, Ladder* ladder)
{
	if(x == -1 && w == -1){
		printLadderDisalvo(ladder);
		return;
	} 	
}




int min(int *g, int *m, int idx, int r, int N)
{
    if (idx == 0)
        return 0;
    int sum1 = 0;
    int sum2 = 0;
    for (int i = idx + 1; i < N; i++)
        sum1 += g[i];
    if (idx != 0)
        for (int i = 0; i < idx; i++)
            sum2 += m[i];
    else
        for (int i = 0; i <= idx; i++)
            sum2 += m[i];

    return (0 > (r - sum1 - sum2)) ? 0 : (r - sum1 - sum2);
}

int max(int *g, int *m, int idx, int r, int N)
{
    if (idx == 0)
        return m[0];
    int sum = 0;
    for (int i = idx + 1; i < N; i++)
    {
        sum += g[i];
    }
    return (m[idx] < r - sum) ? m[idx] : r - sum;
}

int mobileMax(int* g, int idx, int r, int N)
{
    int sum = 0;
    for(int i = idx+1; i < N; i++)sum+= g[i];
    return r - sum;
}

int mobileMin(int* g, int idx, int r, int N)
{
    int sum1 = 0;
    for (int i = idx + 1; i < N; i++)
        sum1 += g[i];
    
    int sum2 = 0;
    for(int i = 0; i < idx; i++)
        sum2 += g[i];

    return (r - sum1 - sum2);
}

bool parity(int *g, int idx, int N)
{
    int sum = 0;
    for (int i = idx + 1; i < N; i++)
        sum += g[i];

    return (sum % 2 == 0) ? true : false;
}
void printPerm(int *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", p[i]);
    }
    printf("\n");
}
void walsh(int *p, int *inverse, int pivot1, int pivot2, bool down)
{
    for (int i = 0; i < 5; i++)
    {
        p[i]--;
        inverse[i]--;
    }
    int a1 = p[pivot1];
    //printf("a1 = %d\n", a1);

    int a2 = p[pivot2];
    //printf("a2 = %d\n", a2);

    int a3 = 0;
    int a4 = 0;
    int b3 = 0;
    int b4 = 0;

    //a1 = a1-1, a2 = a2+1

    if (down)
    {

        a3 = a1 - 1;
        a4 = a2 + 1;
    }

    else
    {
        a3 = a1 + 1;
        a4 = a2 - 1;
    }
    //normally, we swap p[pivot1] with a3 and p[pivot2] with a4.
    //however, if that is the same as swapping p[pivot1] with p[pivot2],
    //i.e if a3 = a2, then we rotate
    if (a3 == a2)
    {
        //decide on how much to offset a3

        down ? a3-- : a3++;
        //b3 is the inverse[a3 offset by 1]
        b3 = inverse[a3];
        //rotate to the left
        if (down)
        {
            p[pivot1] -= 2;
            p[pivot2]++;
            p[b3]++;
        }
        //rotate to the right
        else
        {
            p[pivot1] += 2;
            p[pivot2]--;
            p[b3]--;
        }
        inverse[a3] = pivot1;
        inverse[a2] = b3;
        inverse[a1] = pivot2;
    }
    //rotate with p[pivot1] and p[pivot2] with a3 between them
    else if (a3 == a4)
    {

        b3 = inverse[a3];
        if (down)
        {
            p[pivot1] = p[pivot1] - 2;
            p[pivot2] = p[pivot2] + 1;
            p[b3] = p[b3] + 1;
        }
        else
        {
            p[pivot1] = p[pivot1] + 2;
            p[pivot2] = p[pivot2] - 1;
            p[b3] = p[b3] - 1;
        }
        inverse[a2] = pivot1;
        inverse[a1] = b3;
        inverse[a3] = pivot2;
    }
    //else swap p[pivot1] and p[pivot2] independently
    else
    {

        b3 = inverse[a3];

        if (down)
        {
            p[pivot1] = p[pivot1] - 1;
            p[b3] = p[b3] + 1;
        }

        else
        {
            p[pivot1] = p[pivot1] + 1;
            p[b3] = p[b3] - 1;
        }

        inverse[a3] = pivot1;
        inverse[a1] = b3;
        b4 = inverse[a4];

        if (down)
        {
            p[pivot2] = p[pivot2] + 1;
            p[b4] = p[b4] - 1;
        }
        else
        {
            p[pivot2] = p[pivot2] - 1;
            p[b4] = p[b4] + 1;
        }
        inverse[a4] = pivot2;
        inverse[a2] = b4;
    }
    for (int i = 0; i < 5; i++)
    {
        p[i]++;
        inverse[i]++;
    }
}

int *inverse(int *p, int N)
{
    int *inverse = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++)
    {
        inverse[p[i] - 1] = i + 1;
    }
    return inverse;
}

void runSim(void)
{

    int N = 0;
    int R = 0;
    printf("Enter an N value and an R value separated by a space such that 0 <= R <= N(N-1)/2\n");
    fscanf(stdin, "%d %d", &N, &R);
    //'m' is the bounded composition that binds the inversion vector 'g'upper bound for each inversin on the inversion vector
    int *m = calloc(N - 1, sizeof(int));
    for (int i = 0; i < N - 1; i++)
    {
        m[i] = ((N - 1) - i) <= R ? (N - 1) - i : R;
    }
    //printf("m = \n");
    //printPerm(m, N-1);
    //the inversion vector that is the bounded composition of the permutation and is bounded by the bounded composition 'N'
    int *g = calloc(N - 1, sizeof(int));
    int count = 0;
    //initalize all values in g to either to m[i] if m[i] < R-count, else init to R-count
    for (int i = 0; i < N - 1; i++)
    {
        g[i] = (m[i] < R - count) ? m[i] : R - count;
        count += g[i];
    }
    //printf("g = \n");
    //printPerm(g, N-1);

    //the permutation that is bounded by 'g'
    int *p = calloc(N, sizeof(int));
    //initialize p to the lexicographically largest permutation of size 'N' with R inversions
    count = 0;

    //to create lexicographically largest permutation with r inversions
    int *list = calloc(N, sizeof(int));
    int *set = calloc(N, sizeof(int));

    for (int i = 0; i < N; i++)
        list[i] = N - i;
    //counter for set
    int k = 0;
    //counter for p
    int j = 0;
    for (int i = 0; i < N; i++)
    {
        if (i != N - 1 && m[i] <= R - count)
        {
            p[j] = list[i];
            j++;
            count += m[i];
        }
        else
        {
            set[k] = list[i];
            k++;
        }
    }

    //set  the remaining elemets of p to the values in set
    for (int i = (N - k); i < N; i++)
        p[i] = set[(k - 1) - (i - (N - k))];

    //done with the list and the set
    free(list);
    free(set);

    int *inversE = inverse(p, N);
    int pivot1 = -1;
    int pivot2 = -1;
    do
    {

        pivot1 = -1;
        pivot2 = -1;        
        printPerm(g, N - 1);
        printf("\n");
        setPivots(g, m, &pivot1, &pivot2, N - 1, R);
        //walsh(p, inversE, pivot1,pivot2,true);

    } while (pivot1 != -1 && pivot2 != -1);
}


int getSecondPivot(int* g, int* m, int pivot1, bool parity, int N, int r)
{
	int secondPivot = -1;
	for(int i = pivot1-1; i>=0; i--)
	{
		if(parity == false && g[i] != min(g, m, i, r, N))
		{
			secondPivot = i;
		}
		if(parity == true && g[i] != max(g, m, i, r, N))
		{
			secondPivot = i;
		}
	}
	return secondPivot;
}


void setPivots(int *g, int *m, int *pivot1, int *pivot2, int N, int r)
{
    //to find pivot1 go from index 1 to N-1 in g
    bool paritY = false;
    int secondPivot = -1;
    for (int i = 1; i < N; i++)
    {
        //the first value not at its max or min is the pivot
        paritY = parity(g, i, N);
        if (paritY && g[i] != max(g, m, i, r, N))
        {
	    secondPivot = getSecondPivot(g, m, i, !paritY, N, r);
	    if(secondPivot != -1){
	    	*pivot2 = i;
		*pivot1 = secondPivot;
		break;
	    }
           
        } //end if
        if (!paritY && g[i] != min(g, m, i, r, N))
        {
	    secondPivot = getSecondPivot(g, m, i, !paritY, N, r);
	    if(secondPivot != -1){
	    	*pivot2 = i;
		*pivot1 = secondPivot;
		break;
	    }
            // g[i]--;
        }
    } //end for


  /*  int j = 0;
    int w = 0;
    //start at pivot i - 1 and go right to left
    for (j = *pivot2-1; j > 0; j--)
    {
        // if(g[j] != m[j] && paritY == true && g[j] != 0)
        // {
        //     *pivot1 = j;
        //     break;
        // }
        // if(g[j] != m[j] && paritY == false && g[j] != 0)
         bool maxFlag = false;
         bool minFlag = false;
         //for each element left of j
         for(int k = j-1; k >= 0; k--)
         {
             if(g[k] == m[k])maxFlag = true;
             if(g[k] == 0)minFlag = true;
        }
        if(!maxFlag && minFlag && paritY == true && g[j] != 0)
        {
             w = j;
        }
        if(!minFlag && maxFlag && paritY == false && g[j] != m[j])
        {
             w = j;
        }
        if(maxFlag == true && minFlag == true && paritY == true && g[j] != 0)w = j;
        if(maxFlag == true && minFlag == true && paritY == false && g[j] != m[j])w = j;

        // if (paritY && g[j] != min(g, m, j, r, N))
        // {
        //     *pivot1 = j;
        //     break;
        // }
        // else if (paritY == false && g[j] != max(g, m, j, r, N))
        // {
        //     *pivot1 = j;
        //     break;
        // }
    }*/

    int i = *pivot2;
    *pivot1 = secondPivot;
    int j = *pivot1;

    if(secondPivot == -1)return;
    if (paritY)
    {

        g[i]++;
        g[j]--;
    }
    else
    {
        g[i]--;
        g[j]++;
    }

    //to find pivot2, start from the left of pivot1
    //and go backwards to the beginning of g
    //looking for a j value such that all values to the left of j are at their max
    //or values to the left of j are at their min.
    // if((*pivot2)>1)
    // for(int j = (*pivot2)-1; j >= 0; j--)
    // {
    //     //
    //     bool maxFlag = false;
    //     bool minFlag = false;
    //     for(int k = j-1; k >= 0; k--)
    //     {
    //         if(max(g, m, k, r, N))maxFlag = true;
    //         if(min(g, m, k, r, N))minFlag = true;
    //     }//end for

    //     //if all k < j values in g are at their max or all k < j values in g are at their min
    //     if((maxFlag == true && minFlag == false) || (minFlag == true && maxFlag == false))
    //     {
    //         //we know that j is our pivot2
    //         *pivot1 = j;
    //         break;
    //     }//end if
    // }//end for
    // else
    // {
    //     *pivot1 = 0;
    // }

    // bool even = parity(g,*pivot2, N);
    // int i = *pivot1;
    // int j = *pivot2;
    // if(even)
    // {
    //     g[i]--;
    //     g[j]++;
    // }
    // else
    // {
    //     g[i]++;
    //     g[j]--;
    // }

} //edn func

int main(int argc, char *argv[])
{
    runSim();
}
// procedure NextPerm(p,inverse:array; pivot1,pivot2:integer; down:boolean)
//  a1:=p[pivot1]; a2:=p[pivot2];
//  if (down) then {p[pivot1] decreases and p[pivot2] increases, normally by 1}
//  a3:=a1-1; a4:=a2+1
//  else {p[pivot1] increases and p[pivot2] decreases, normally by 1}
//  a3:=a1+1; a4:=a2-1
//  end if;
// { Normally p[pivot1]=a1 swaps with the element a3 and p[pivot2]=a2 with a4, but if that would
// make p[pivot1] swap with p[pivot2], then instead they rotate with a third element of p. }
//

//  inverse[a3]:=pivot1; inverse[a2]:=b3; inverse[a1]:=pivot2

//else if (a3=a4) then {p[pivot1] and p[pivot2] differ by 2 and would converge;
// instead they rotate with the element a3 between them in value. }
//  b3:=inverse[a3]; { p[pivot1] and p[pivot2] rotate with p[b3] }
//  if (down) then p[pivot1]:=p[pivot1]-2; p[pivot2]:=p[pivot2]+1; p[b3]:=p[b3]+1
//  else p[pivot1]:=p[pivot1]+2; p[pivot2]:=p[pivot2]-1; p[b3]:=p[b3]-1
//  end if;
//  inverse[a2]:=pivot1; inverse[a1]:=b3; inverse[a3]:=pivot2
//end if
// end NextPerm.
