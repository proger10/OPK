#include <stdio.h>

int main(){
	int all = 0;
	int good = 0;
	for (int i = 0; i < 1000000; i++){
		all++;
		int ticket = i;
		int side1 = 0;
		int side2 = 0;

		side1 += ticket % 10;
		ticket /= 10;
		side1 += ticket % 10;
		ticket /= 10;
		side1 += ticket % 10;
		ticket /= 10;

		side2 += ticket % 10;
		ticket /= 10;
		side2 += ticket % 10;
		ticket /= 10;
		side2 += ticket % 10;
		if (side1 == side2){
			good++;
		}
	}

	printf("Number of Good tickets is %d\n", good);
	printf("Percentage of Good tickets is %.2lf\n", (double)100 * good / all);

	return 0;
}