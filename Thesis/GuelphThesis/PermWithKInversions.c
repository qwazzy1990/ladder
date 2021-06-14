#include <stdio.h>
#include <stdlib.h>




int numPermsWithKInversions(int n, int k);
int memo[100][100];

int numPermsWithKInversions(int n, int k)
{
	if(n ==0)return 0;
	if(k == 0)return 1;
	if(memo[n][k] != 0)
		return memo[n][k];
	
	int sum = 0;
	for(int i = 0; i <= k; i++){
		if(i <= n-1)
			sum+=numPermsWithKInversions(n-1, k-i);
	}

	memo[n][k] = sum;
	return sum;


}


int main()
{
	int n = 5;
	int k = 3;
	printf("Number of permutations of length %d with %d inversions = %d\n", n, k, numPermsWithKInversions(n, k)); 
	return 0;
}
