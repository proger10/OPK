#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

int main(){
	printf("Square (A*x^2+B*x+C=0) equantion solver.\n");
	double a, b, c;
	printf("A = ");
	scanf("%lf", &a);
	printf("B = ");
	scanf("%lf", &b);
	printf("C = ");
	scanf("%lf", &c);
	printf("Equantion: %g*x^2%+g*x%+g=0\n", a, b, c);
	printf("Roots: ");

	// ==0 for floats???
	if (a == 0){
		//Linear equantion
		if (b == 0){
			if (c == 0){
				printf("any\n");
			}
			else{
				printf("no\n");
			}
		}
		else{
			printf("%g", -c / b);
		}
	}
	else{
		//Square equantion

		double d = b*b - 4 * a*c;
		if (d < 0){
			printf("no\n");
		}
		if (d == 0){
			printf("%g\n", -b/(2*a));
		}
		if (d > 0){
			printf("%g; %g\n", (-b + sqrt(d)) / (2 * a), (-b - sqrt(d)) / (2 * a));
		}
	}
}