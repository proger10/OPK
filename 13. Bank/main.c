#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	printf("Bank percent calculator.\n");

	printf("Input money: ");
	//%g - €рослав таки был прав
	double money;
	scanf("%lf", &money);
	if (money < 1){
		printf("U R MAD!!!\n");
		return 0;
	}

	printf("Deposit time (months): ");
	int time;
	scanf("%d", &time);
	if (time < 1){
		printf("U R MAD!!!\n");
		return 0;
	}

	printf("Interest rate (%): ");
	double a;
	scanf("%lf", &a);
	a = a/ 100;
	if (a <= 0){
		printf("Bad bank =(\n");
		return 0;
	}
	printf("\n");
	printf("Month\t| Simple interest\t| Compound interest\n");
	printf("-------------------------------------------------------------------\n");
	for (int i = 0; i <= time; i++){
		double simple, comp;
		simple = money * (1 + a*i/12);
		comp = money; //* pow(1+a/, );
		printf("%d\t| %.2lf\t\t| %.2lf\n", i, simple, comp);
	}
	return 0;
}