#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
	int good = 0;
	int arr[28] = {};

	for (int i = 0; i < 1000; i++){
		int ticket = i;
		int side = 0;

		side += ticket % 10;
		ticket /= 10;
		side += ticket % 10;
		ticket /= 10;
		side += ticket % 10;
		ticket /= 10;

		arr[side]++;
	}

	for (int i = 0; i < 9 + 9 + 9 + 1; i++){
		good += arr[i] * arr[i];
	}


	printf("Number of Good tickets is %d\n", good);
	printf("Percentage of Good tickets is %.2lf\n", (double)100 * good / 1000000);

	return 0;
}