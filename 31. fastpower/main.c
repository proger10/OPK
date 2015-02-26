#include <stdio.h>



double fastpow(double x, int n){
	double result = (double) 1;
	while (n != 0){
		if (n % 2 == 1){
			result *= x;
		}
		x *= x;
		n >>= 1;
	}
	return result;
}

int main(){
	int b = 6;
	double x = 2;
	double res = fastpow(x, b);
	printf("%g %g %d\n", res, x, b);
}