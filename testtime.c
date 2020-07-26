#include<stdio.h>
#include <stdlib.h>
#include<time.h>
int main() {
	int i;
	double total_time;
	clock_t start, end;
	start = clock();
	//time count starts 
	srand(time(NULL));
	for (i = 0; i < 25000; i++) {
		printf("random_number[%d]= %d\n", i + 1, rand());
	}
	end = clock();
	//time count stops 
	total_time = ((double) (end - start)) / CLOCKS_PER_SEC;
	//calulate total time
	printf("\nTime taken to print 25000 random number in seconds is is: %f\n", total_time);
	return 0;
}