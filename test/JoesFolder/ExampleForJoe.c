#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int recursiveT(int n);
int recursiveFib(int n);
double formulaT(int n);
double formulaFib(int n);
int fact(int n);

int fact(int n)
{
    int count = 1;
    for(int i = 1; i <= n; i++)
    {
        count*=i;
    }
    return count;
}
int recursiveT(int n)
{
   if(n == 0)return 0;
   if(n == 1)return 0;
   int count = recursiveT(n-1) + recursiveT(n-2) + 1;
   return count; 
}

int recursiveFib(int n)
{
    if(n == 0)return 0;
    if(n == 1)return 1;
    int count = recursiveFib(n-1) + recursiveFib(n-2);
    return count;
}


double formulaT(int n)
{
    double a = 1/sqrt(5);
    double b = pow((1+sqrt(5))/2, n+1);
    double c = pow((1-sqrt(5))/2, n+1);

    //equivalent to 'return formulaFib(n+1) - 1';
    return (a*(b-c))-1;
}

double formulaFib(int n)
{
    double a = 1/sqrt(5);
    double b = pow((1+sqrt(5))/2, n);
    double c = pow((1-sqrt(5))/2, n);
    return (a*(b-c));
}


int main()
{
    for(int i = 2; i <= 10; i++)
    {
        printf("\n\n------------------------------------------------------------------------------------------------------------------------\n");
        printf("N is %d\n", i);
        printf("Out of all the %d factorial ladders of order N=%d, the number of ladders with a height of one is %d\n",fact(i), i, recursiveT(i));
        printf("The %dth Fibonacci number is %f\n", i+1, formulaFib(i+1));
        printf("The result of the formula for calculating all ladders with a height of one given N=%d is %f\n",i, formulaT(i));
        printf("------------------------------------------------------------------------------------------------------------------------\n");

    }
   printf("---------------------------------------------------------------------------------------------------------------------------------\n");

    printf("Let K be the number of ladders with a height of one given some N value. Let F be the N+1th fibonacci number. Notice how K = N-1.\n");
    printf("---------------------------------------------------------------------------------------------------------------------------------\n");

    return 0;    
}