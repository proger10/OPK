#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void skip_garbage() {
	int ch;
	do {
		ch = getchar();
	} while (ch != '\n');
}

int read_int(const char * request) {
	for (;;) {
		printf("%s", request);
		int num;
		int res = scanf("%d", &num);
		skip_garbage();
		if (res == 1) {
			return num;
		}
	}
}

double read_double(const char * request) {
	for (;;) {
		printf("%s", request);
		double num;
		int res = scanf("%lg", &num);
		skip_garbage();
		if (res == 1) {
			return num;
		}
	}
}

int main(){
	printf("Bank percent calculator.\n");

	double money = read_double("Your money: ");
	if (money < 1){
		printf("U R MAD!!!\n");
		return 0;
	}

	int time = read_int("Deposit time (months): ");
	if (time < 1){
		printf("U R MAD!!!\n");
		return 0;
	}

	double a = read_double("Interest rate (%): ");
	a = a / 100;
	if (a <= 0){
		printf("Bad bank =(\n");
		return 0;
	}


	printf("\n");
	printf("Month\t| Simple interest\t| Compound interest\n");
	printf("-------------------------------------------------------------------\n");

	for (int i = 0; i <= time; i++){
		double simple, comp;
		simple = money * (1 + a*i / 12);
		comp = money * pow(1+a/12, i);
		printf("%d\t| %.2lf\t\t| %.2lf\n", i, simple, comp);
	}
	return 0;
}