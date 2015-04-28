#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main() {
	printf("List all prime numbers from 1 to N\n");
	printf("N = ");
	int n;
	scanf("%d", &n);

	bool* arr = (bool*)calloc(n+1, sizeof(bool));
	arr[0] = arr[1] = true;
	
	for (int i = 2; i <= n; i++){
		if (!arr[i]){
			printf("%d ", i);
			for (int j = 2 * i; j <= n; j += i){
				arr[j] = true;
			}
		}
	}

	free(arr);
	return 0;

}