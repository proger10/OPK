#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	printf("List all prime numbers from 1 to N\n");
	printf("N = ");
	int n;
	scanf("%d", &n);

	for (int num = 1; num <= n; num++){
		char is_prime = 1;

		for (int i = 2; i*i <= num; i++){
			if (0 == (num % i)){
				is_prime = 0;
				break;
			}
		}

		if (is_prime){
			printf("%d ", num);
		}
	}
}