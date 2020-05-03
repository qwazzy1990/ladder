#include <stdio.h>


int main( void )
{
	double vals[4];
	vals[0] = 0.700000;
	vals[1] = 0.891304;
	vals[2] = 0.987395;
	vals[3] = 1.021558;
	printf("vals are: %f %f %f %f\n", vals[0], vals[1], vals[2], vals[3]);
	double num = 0;
	for(int i = 0; i < 4; i++)num += vals[i];
	double mean = num/4;
	printf("mean is %f\n", mean);
	num = 0;
	for(int i = 0; i < 4; i++)
	{
		num += (vals[i] - mean)*(vals[i]-mean); 
	}
        double var = num/3;
	printf("variance is %f\n", var);	

}
