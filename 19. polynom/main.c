#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 

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

double polynom(double x, double a[], int n)
{
	double result = 0;
	for (int i = n; i >= 0; i--){
		result = a[i] + result*x;
	}
	return result;
}

int main(){
	printf("Value of polynom at x\n");
	int n = read_int("Polymon power: ");
	if (n < 0){
		printf("WAT???\n");
		return 0;
	}

	double arr[256] = { 0 };
	printf("Scanning for %d coeffs\n", n+1);
	for (int i = n; i >= 0; i--){
		arr[i] = read_double("Next coeff: ");
	}

	for (int i = n; i >= 0; i--){
		printf("%+g*x^%d", arr[i], i);
	}
	printf("\n\n");

	double x = read_double("x = ");
	printf("Polynom at x=%g is %g\n", x, polynom(x, arr, n));

	return 0;
}