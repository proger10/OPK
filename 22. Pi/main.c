#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

int main(){
	srand((time(NULL)) % INT_MAX);
	int all = 0, good = 0;
	//int randtest[RAND_MAX+1] = {0};
	for (int i = 0; i < 100000000; i++){
		all++;

		int r = rand();
		//randtest[r]++;
		double x = 2.0*r / RAND_MAX - 1;

		r = rand();
		//randtest[r]++;
		double y = 2.0*r / RAND_MAX - 1;

		if (x*x + y*y <= 1){
			good++;
		}
	}
	//for (int i = 0; i <= RAND_MAX; i++)
	//	printf("%d\n", randtest[i]);
	printf("Pi = %lf for current phase of the moon\n", 4*((float)good) / all);
	return 0;
}