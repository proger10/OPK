#define _CRT_SECURE_NO_WARNINGS
#define ARR_MAX 1024
#include <stdio.h>

void get_sums(unsigned long long sums[], unsigned long long n){
	if (n == 1){
		for (int i = 0; i <= 9; i++){
			sums[i] = 1;
		}
		return;
	}
	unsigned long long sums2[ARR_MAX] = { 0 };
	get_sums(sums2, n / 2);
	for (unsigned long long i = 0; i <= 9 * n / 2; i++){
		for (unsigned long long j = 0; j <= 9 * n / 2; j++){
			sums[i + j] += sums2[i] * sums2[j];
		}
	}
}

int main(){
	unsigned long long sums[ARR_MAX] = { 0 };
	get_sums(sums, 16);
	double sum = 0;
	for (unsigned long long i = 0; i <= 16*9; i++){
		sum += (1.0*sums[i] / 1E16) * (1.0*sums[i] / 1E16);
	}
	printf("%lf\n", sum);
	return 0;
}