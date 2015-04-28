#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>

double pi_acc(double pi){
	//M_PI*(1+-acc)=pi
	return fabs((pi / M_PI - 1)*100);
}

int main(){
	srand((time(NULL)) % INT_MAX);
	long long int limit = 1L;
	for (int j = 0; j <= 20; j++){
		int all = 0, good = 0;
		for (int i = 0; i < limit; i++){
			all++;

			int r = rand();
			double x = 2.0*r / RAND_MAX - 1;

			r = rand();
			double y = 2.0*r / RAND_MAX - 1;

			if (x*x + y*y <= 1){
				good++;
			}
		}
		double pi = 4 * ((double)good) / all;
		printf("Rand_Pi = %lf for %d zeros. ", pi, j);
		printf("Difference %.4lf%% of Pi\n", pi_acc(pi));
		limit *= 10;
	}
	return 0;
}